from gfx import processGraphics
from rotate import rotateScrollingBackgrounds
from tools import makeDirs
from pathlib import Path


def addSection(text, number):
    section = ".rodata" + str(number)
    text += '.section "' + section + '" superfree'
    text += "\n\n"
    return text


def endSection(text):
    text += ".ends\n\n"
    return text


sectionSizeLimit = 0x8000


def writeHeader(data):
    source = '#include <snes.h>\n\n'
    for d in data:
        label = d["prefix"] + "_" + d["file"].replace(".", "_")
        source += "extern char " + label + ", "
        source += label + "_end;\n"
        source += "#define " + label + "_size "
        source += "(&" + label + "_end - &" + label + ")\n"
    open("src/data.h", "w").write(source)


def writeData(data):
    asmText = '.include "hdr.asm"\n\n'
    sectionNumber = 1
    bytesUsed = 0
    asmText = addSection(asmText, sectionNumber)
    for entry in data:
        file = Path("data/" + entry["file"])
        size = file.stat().st_size
        if size >= sectionSizeLimit:
            print(file, "too big!", size)
            exit(1)
        # if spilling into next section, make new one
        if (bytesUsed + size) >= sectionSizeLimit:
            asmText = endSection(asmText)
            sectionNumber += 1
            asmText = addSection(asmText, sectionNumber)
            bytesUsed = size
        bytesUsed += size
        name = entry["file"]
        label = entry["prefix"] + "_" + name.replace(".", "_")
        asmText += label + ":\n"
        asmText += '.incbin "data/' + name + '"\n'
        asmText += label + "_end:\n\n"
    asmText = endSection(asmText)

    open("data.asm", "w").write(asmText)


makeDirs()

data = []
processGraphics(data)
data = sorted(data, key=lambda x: x["file"])
writeData(data)
writeHeader(data)
rotateScrollingBackgrounds()