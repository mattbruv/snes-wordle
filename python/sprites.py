from math import ceil
from pathlib import Path
from PIL import Image
from numpy import mat

C_SOURCE = """#include <snes.h>
#include "spriteData.h"
#include "data.h"

void initSpriteData()
{{
    {initCode}
}}
"""

C_HEADER = """#ifndef SPRITE_DATA_H
#define SPRITE_DATA_H

#include <snes.h>
#include "sprite.h"

{defAnims}
{defAnimFrames}

void initSpriteData();

#endif
"""

def getSprites():
    anims = {}

    files = Path.cwd().rglob(pattern="gfx/sprites/fighters/*/*/*.bmp")
    for f in files:
        p = f.parts
        person = p[6]
        anim = p[7]
        file = p[-1].split('.')[0]
        if person not in anims:
            anims[person] = {}
        if anim not in anims[person]:
            anims[person][anim] = []
        # get all the info here
        info = {}
        info["name"] = file
        info["path"] = str(f)
        img = Image.open(str(f))
        width, height = img.size
        info["width"] = width
        info["height"] = height
        anims[person][anim].append(info)
        #print(person, anim, p, file)
        #print(info)
    return anims

def writeHeader(anims):
    #print(anims)
    header = C_HEADER
    defAnims = []
    defAnimFrames = []

    for person in anims:
        defAnims.append("// " + person + " animations")
        #defAnimFrames.append("// " + person + " frames")
        for anim in anims[person]:
            defAnims.append("Animation " + "anim_" + person + "_" + anim + ";")
            size = str(len(anims[person][anim]))
            defAnims.append("AnimFrame " + "frames_" + person + "_" + anim + "[" + size + "];")
            for info in anims[person][anim]:
                frame = info["name"]
                #defAnimFrames.append("AnimFrame animframe_" + frame + ";")
            print(person, anim)
        defAnims.append("")
        defAnimFrames.append("")
    
    header = header.format(
        defAnims='\n'.join(defAnims),
        defAnimFrames='\n'.join(defAnimFrames)
    )

    return header

def writeSource(anims):
    source = C_SOURCE
    initCode = ["// queriendo quedarse en Monaco"]


    for person in anims:
        for anim in anims[person]:
            frameIndex = 0
            for info in anims[person][anim]:
                frame = info["name"]
                label = "frames_" + person + "_" + anim + "[" + str(frameIndex) + "]"
                frameIndex += 1
                initCode.append("    " + label + ".centerX = 0;")
                initCode.append("    " + label + ".centerY = 0;")

                initCode.append("    " + label + ".colCount = " + str(ceil(info["width"] / 16)) + ";")
                initCode.append("    " + label + ".rowCount = " + str(ceil(info["height"] / 16)) + ";")

                initCode.append("    " + label + ".picAddr = &gfx_" + frame + "_pic;" )
                initCode.append("    " + label + ".palAddr = &gfx_" + frame + "_pal;" )
                initCode.append("    " + label + ".picSize = gfx_" + frame + "_pic_size;" )
                initCode.append("    " + label + ".palSize = gfx_" + frame + "_pal_size;" )

                initCode.append("")
                #print(frame)
            initCode.append("")
            print(person, anim)

    source = source.format(initCode="\n".join(initCode))
    return source


def genSpriteFiles():
    anims = getSprites()
    header = writeHeader(anims)
    open("src/spriteData.h", "w").write(header)
    source = writeSource(anims)
    open("src/spriteData.c", "w").write(source)

genSpriteFiles()