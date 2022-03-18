import os
from pathlib import Path

home = os.environ.get("PVSNESLIB_HOME")

# $(PVSNESLIB_HOME)/devkitsnes/tools/gfx2snes


def makeDirs():
    Path("data/").mkdir(parents=True, exist_ok=True)


def moveFilesToData(rootPath, fileTypes):
    for t in fileTypes:
        fs = Path(rootPath).rglob("*" + t)
        for f in fs:
            name = f.name
            f.replace("data/" + name)


def toolPath(toolName):
    return home + "/devkitsnes/tools/" + toolName


GFX_TOOL = toolPath("gfx2snes")