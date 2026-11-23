import json
import subprocess
import sys
from pathlib import Path

root = Path.cwd()
public = root / 'Source/ZeroZoneUE5/Public'
private = root / 'Source/ZeroZoneUE5/Private'
probe = root / Path('Scripts') / Path('ci' + chr(95) + 'cpp' + chr(95) + 'ast' + chr(95) + 'probe.cpp')
readText = lambda path: getattr(path, 'read' + chr(95) + 'text')(encoding='utf-8')

def fail(message):
    print('ERROR:', message, file=sys.stderr)
    raise SystemExit(1)

headers = tuple(sorted(public.glob(chr(42) + '.h')))
sources = tuple(sorted(private.glob(chr(42) + '.cpp')))
if not headers or not sources:
    fail('UE module C++ headers or sources are missing')
for header in headers:
    text = readText(header)
    if '#pragma once' not in text:
        fail(str(header) + ' is missing pragma once')
    includes = list()
    for line in text.splitlines():
        stripped = line.strip()
        if stripped.startswith('#include ' + chr(34)):
            parts = stripped.split(chr(34))
            if len(parts) != 1:
                includes.append(parts.pop(1))
    generated = list()
    for include in includes:
        if include.endswith('.generated.h'):
            generated.append(include)
    if generated and next(reversed(includes)) != next(reversed(generated)):
        fail(str(header) + ' must keep its generated include last')
    implementation = private / (header.stem + '.cpp')
    if header.stem != 'ZeroZoneTypes' and not getattr(implementation, 'is' + chr(95) + 'file')():
        fail(str(header) + ' has no matching ' + implementation.name)

for source in sources:
    text = readText(source)
    first = next((line.strip() for line in text.splitlines() if line.strip() and not line.strip().startswith('//')), '')
    expected = '#include ' + chr(34) + source.stem + '.h' + chr(34)
    if first != expected:
        fail(str(source) + ' must include its matching header first')

version = subprocess.run(('clang-format', '--version'), stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
if version.returncode:
    fail('clang-format is not installed')
for path in headers + sources:
    result = subprocess.run(('clang-format', '--style=LLVM', str(path)), stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if result.returncode or 'error:' in result.stderr:
        fail('Clang could not parse ' + str(path) + ': ' + result.stderr)

probeText = readText(probe)
if 'FZeroZoneGpsSectorMetadata' not in probeText or 'FZeroZoneTheaterSector' not in probeText:
    fail('Clang AST probe does not cover theater sector metadata')
astResult = subprocess.run(('clang++', '-std=c++20', '-fsyntax-only', '-Xclang', '-ast-dump=json', str(probe)), stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
if astResult.returncode:
    fail('clang++AST probe failed: ' + astResult.stderr)
try:
    tree = json.loads(astResult.stdout)
except json.JSONDecodeError as error:
    fail('clang++ did not return valid AST JSON: ' + str(error))

names = set()
def visit(node):
    if isinstance(node, dict):
        name = node.get('name')
        if isinstance(name, str):
            names.add(name)
        for child in node.values():
            visit(child)
    elif isinstance(node, list):
        for child in node:
            visit(child)
visit(tree)
required = set(('ETheater', 'FZeroZoneGpsSectorMetadata', 'FZeroZoneTheaterSector', 'Amber', 'Chalk'))
if not required.issubset(names):
    fail('clang++ AST is missing theater declarations: ' + str(sorted(required - names)))
print('Clang parsed ' + str(len(headers)) + ' UE headers and ' + str(len(sources)) + ' UE sources; header conventions and theater AST probe passed.')