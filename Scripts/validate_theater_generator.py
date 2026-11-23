import ast
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
path = ROOT / "Scripts/build_theaters.py"
source = path.read_text(encoding="utf-8")
ast.parse(source, filename=str(path))
required = [
    "AmberCanal", "ChalkReach", "bake_theater_landscape", "theater_height_cm",
    "levels.new_level(package, True)", "Refusing to overwrite", "PlayerStart",
    "SkyAtmosphere", "ExponentialHeightFog", "volumetric_fog",
    "M_Dune", "M_Silt", "M_Chalk", "M_Cast", "M_CanalWater",
]
missing = [term for term in required if term not in source]
if missing:
    raise SystemExit("Theater generator contract missing: " + ", ".join(missing))
if "30.5800" in source or "32.2700" in source:
    raise SystemExit("Generator must remain fictional and must not encode real-world coordinates")
print("Theater generator parses and declares both fictional partitioned map recipes, material slots and save guards.")
