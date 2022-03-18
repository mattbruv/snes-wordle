import subprocess
from pathlib import Path

from tools import GFX_TOOL, moveFilesToData

defaultArgs = ["-pr", "-pc16", "-n", "-gs8", "-pe0", "-fbmp", "-m"]

scrollingArgs = [
    "-mR!", # No tile reduction (not advised)
    #"-pr",  # Rearrange palette, and preserve palette numbers in the tilemap
    "-pc16",  # The number of colors to use: 16
    "-n",  # no border
    "-gs8",  # Size of image blocks in pixels: 8
    "-pe0",  # The palette entry to add to map tiles (0 to 16)
    "-fbmp",  # convert a bmp
    "-mp",  # Convert the whole picture with high priority
    "-m32p"  # Generate tile map organized in pages of 32x32 (good for scrolling)
]

# -pc4 = 4bpp
fontArgs = "-n -gs8 -po2 -pc16 -mR! -m! -p!".split()

spriteArgs = "-gs16 -pc16 -po16 -n".split()
# gs16 because we have 16 pix width
# pc16 because we can only use 16 colors
# po16 because we are going to use only one palette of 16 colors
# n because we do not have a border around the sprites

graphics = [  #
    {
        "glob": "backgrounds/*.bmp",
        "args": defaultArgs
    },  #
    {
        "glob": "backgrounds/static/*.bmp",
        "args": defaultArgs
    },
    {
        "glob": "backgrounds/scrolling/*.bmp",
        "args": scrollingArgs
    },  #
    {
        "glob": "sprites/fighters/**/*.bmp",
        "args": spriteArgs
    },
    {
        "glob": "fonts/*.bmp",
        "args": fontArgs
    }
]

filetypes = [".map", ".pal", ".pic"]

gfxPath = "gfx/"


def processOutput(data):
    for filetype in filetypes:
        files = Path(gfxPath).rglob("*" + filetype)
        for f in files:
            name = f.name
            entry = {"file": name, "prefix": "gfx"}
            data.append(entry)
            f.replace("data/" + name)


def processGraphics(data):
    print("Processing graphics...")

    for group in graphics:
        files = Path("gfx/").rglob(group["glob"])
        for f in files:
            command = [GFX_TOOL] + group["args"] + [str(f)]
            subprocess.run(command)

    processOutput(data)
