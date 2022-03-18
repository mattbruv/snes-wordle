from operator import indexOf
from pathlib import Path


def flatten(t):
    return [item for sublist in t for item in sublist]

def rotatePic(data):
    if len(data) != 30720:
        exit("ERROR: PICTURE NOT 32x30!!")

    rotated = []
    width = 32*8*4

    for x in range(0, 32):
        for y in range(0, 30):
            offset = y * width + (x * 8 * 4)
            ns = data[offset:offset+8*4]
            rotated.append(ns)
            #print(x, y, offset, ns)
            pass

    return flatten(rotated)

def rotateMap(data):
    if len(data) != 32*30*2:
        exit("ERROR: MAP NOT 32x30!!")

    rotated = []
    width = 32*2

    for x in range(0, 32):
        for y in range(0, 30):
            offset = y * width + (x * 2)
            ns = data[offset:offset+2]
            rotated.append(ns)
            #print(x, y, offset, ns)
            pass

    return flatten(rotated)


def genMap():
    data = []
    for y in range(0, 32):
        for x in range(0, 32):
            n = 1 << 5
            n = n << 8
            index = y + x * 30
            n |= index
            #print(x, y, n, index, "{0:16b}".format(n))
            high = n >> 8
            low = n & 0x00ff
            data.append(low)
            data.append(high)
    return data

def genMapRight():
    data = []
    for y in range(0, 32):
        for x in range(0, 32):
            indexOffset = 0
            tempX = x
            if x == 0:
                indexOffset = 960
            elif x == 1:
                indexOffset = 960 + 30
            else:
                # TODO: this isn't needed, remove this code
                # This resulted in some weird wrapping shared columns
                # tempX -= 1
                tempX += 0
            n = 1 << 5
            n = n << 8
            index = y + tempX * 30 + indexOffset + 0
            n |= index
            #print(x, y, n, index, "{0:16b}".format(n))
            high = n >> 8
            low = n & 0x00ff
            data.append(low)
            data.append(high)
    return data

"""
with open("../data/lake_01.pic", "rb") as f:
    data = list(f.read())
data = rotatePic(data)
open("../data/lake_01.pic", "wb").write(bytes(data))

newMap = bytes(genMap())
open("../data/lake_01.map", "wb").write(newMap)
"""

def getPath(path: Path):
    return "data/" + path.name.replace(".bmp", ".pic")

def rotateScrollingBackgrounds():
    bgs = Path("gfx/backgrounds/scrolling/").rglob("*.bmp")
    bgs = list(map(getPath, bgs))
    for bg in bgs:
        print("rotate:", bg)
        data = list(open(bg, "rb").read())
        rot = bytes(rotatePic(data))
        open(bg, "wb").write(rot)
        open(bg.replace(".pic",".map"), "wb").write(bytes(genMap() + genMapRight()))

