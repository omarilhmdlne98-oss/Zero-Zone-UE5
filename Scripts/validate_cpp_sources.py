from __future__ import annotations

import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PUBLIC = ROOT / "Source/ZeroZoneUE5/Public"
PRIVATE = ROOT / "Source/ZeroZoneUE5/Private"
PROBE = ROOT / "Scripts/ci_cpp_ast_probe.cpp"


def fail(message: str) -> None:
    print("ERROR:", message, file=sys.stderr)
    raise SystemExit(1)


def code_only(text: str) -> str:
    pattern = r'//[^\\n]*|/\\*[\\s\\S]*?\\*/|"(?:\\\\.|[^"\\\\])*"|\'(?:\\\\.|[^\'\\\\])*\''
    return re.sub(pattern, "", text)


headers = sorted(PUBLIC.glob("*.h"))
sources = sorted(PRIVATE.glob("*.cpp"))
if not headers or not sources:
    fail("UE module C++ headers or sources are missing")

for header in headers:
    text = header.read_text(encoding="utf-8")
    if "#pragma once" not in text:
        fail(f"{header.relative_to(ROOT)} is missing #pragma once")
    includes = re.findall(r'^\\s*#include\\s+"([^\"]+)"', text, flags=re.MULTILINE)
    generated = [inc for inc in includes if inc.endswith(".generated.h")]
    if generated and includes[-1] != generated[-1]:
        fail(f"{header.relative_to(ROOT)} must keep its generated include last")
    implementation = PRIVATE / (header.stem + ".cpp")
    if header.stem != "ZeroZoneTypes" and not implementation.is_file():
        fail(f"{header.relative_to(ROOT)} has no matching {implementation.name}")

for source in sources:
    text = source.read_text(encoding="utf-8")
    first = next((line.strip() for line in text.splitlines() if line.strip() and not line.strip().startswith("//")), "")
    expected = f'#include "{source.stem}.h"'
    if first != expected:
        fail(f"{source.relative_to(ROOT)} must include {source.stem}.h first")
    stack = []
    pairs = {")": "(", "]": "[", "}": "{"}
    for char in code_only(text):
        if char in "([{":
            stack.append(char)
        elif char in ")]}" and (not stack or stack.pop() != pairs[char]):
            fail(f"Unbalanced C++ delimiters in {source.relative_to(ROOT)}")
    if stack:
        fail(f"Unclosed C++ delimiters in {source.relative_to(ROOT)}")

clang_format = subprocess.run(["clang-format", "--version"], capture_output=True, text=True)
if clang_format.returncode:
    fail("clang-format is not installed")
for path in headers + sources:
    result = subprocess.run(["clang-format", "--dry-run", "--style=LLVM", str(path)], capture_output=True, text=True)
    diagnostics = result.stdout + result.stderr
    if result.returncode or re.search(r"(^|\\n).*error:", diagnostics):
        fail(f"Clang could not parse {path.relative_to(ROOT)}: {diagnostics}")

probe_source = PROBE.read_text(encoding="utf-8")
if "FZeroZoneGpsSectorMetadata" not in probe_source or "FZeroZoneTheaterSector" not in probe_source:
    fail("Clang AST probe does not cover the theater sector metadata")
ast_result = subprocess.run(
    ["clang++", "-std=c++20", "-fsyntax-only", "-Xclang", "-ast-dump=json", str(PROBE)],
    capture_output=True,
    text=True,
)
if ast_result.returncode:
    fail("clang++ AST probe failed: " + ast_result.stderr)
try:
    tree = json.loads(ast_result.stdout)
except json.JSONDecodeError as error:
    fail(f"clang++ did not return valid AST JSON: {error}")

names: set[str] = set()
def visit(node: object) -> None:
    if isinstance(node, dict):
        name = node.get("name")
        if isinstance(name, str):
            names.add(name)
        for child in node.values():
            visit(child)
    elif isinstance(node, list):
        for child in node:
            visit(child)
visit(tree)
required_ast_names = {"ETheater", "FZeroZoneGpsSectorMetadata", "FZeroZoneTheaterSector", "Amber", "Chalk"}
if not required_ast_names.issubset(names):
    fail("clang++ AST is missing expected theater declarations: " + str(sorted(required_ast_names - names)))

print(f"Clang parsed {len(headers)} UE headers and {len(sources)} UE sources; header conventions and theater AST probe passed.")
