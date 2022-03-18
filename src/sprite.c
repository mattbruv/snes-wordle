#include <snes.h>
#include "data.h"
#include "sprite.h"
#include "spriteData.h"

#define SPRITE_ADDR 0x6000
#define PAL_INDEX 0

typedef struct SpriteFighter
{
    u8 id;
    u16 address;
    u8 paletteNum;

    u8 xPos;
    u8 yPos;

    Animation* currentAnim;
} SpriteFighter;

SpriteFighter spritePlayer1;
SpriteFighter spritePlayer2;

void loadSprite(SpriteFighter* sprite)
{
    Animation* anim = sprite->currentAnim;
    AnimFrame* frame = &anim->frames[anim->frameCounter];

    //consoleNocashMessage("pic addr: %hu, pal addr: %hu\n",
    //                    (u16)anim->frames[anim->frameCounter].picAddr, (u16)&frame->palAddr);

    oamInitGfxSet(frame->picAddr, frame->picSize, frame->palAddr, frame->palSize,
                  sprite->paletteNum, sprite->address, OBJ_SIZE8_L16);
    //dmaCopyCGram(frame->palAddr, 0x10 * (sprite->paletteNum + 8), frame->palSize);

    // consoleNocashMessage("pal id: %hu\n", (u16)sprite->paletteNum);

    u16 x = 0;
    u16 y = 0;
    u16 spriteCount = 0;
    u16 idOffset = sprite->id * 50;
    u16 id = 0;
    u16 rows = 0;
    u16 offset = 0;

    for (y = 0; y < frame->rowCount; y++)
    {
        for (x = 0; x < frame->colCount; x++)
        {
            rows = (spriteCount * 2) / 16;
            id = spriteCount * 4 + (idOffset * 4);
            offset = ((spriteCount * 2)) + rows * 16 + (sprite->id * 16 * 8);
            oamSet(id, x * 16 + sprite->xPos, y * 16 + sprite->yPos, 3, 0, 0, offset,
                   sprite->paletteNum);
            oamSetEx(id, OBJ_LARGE, OBJ_SHOW);
            spriteCount++;
        }
    }
    //WaitForVBlank();
}

void initSprites()
{
    initSpriteData();

    anim_evan_idle.frames = frames_evan_idle;
    anim_evan_idle.totalFrames = 10;
    anim_evan_idle.waitThreshold = 3;
    anim_evan_idle.frameCounter = 0;
    anim_evan_idle.waitCounter = 0;
    anim_evan_idle.reversable = 1;
    anim_evan_idle.frameDirection = 1;

    anim_matt_idle.frames = frames_matt_idle;
    anim_matt_idle.totalFrames = 4;
    anim_matt_idle.waitThreshold = 4;
    anim_matt_idle.frameCounter = 0;
    anim_matt_idle.waitCounter = 0;
    anim_matt_idle.reversable = 1;
    anim_matt_idle.frameDirection = 1;

    spritePlayer1.xPos = 55;
    spritePlayer1.yPos = 85;
    spritePlayer1.id = 0;
    spritePlayer1.address = SPRITE_ADDR;
    spritePlayer1.paletteNum = 0;
    spritePlayer1.currentAnim = &anim_matt_idle;

    spritePlayer2.xPos = 155;
    spritePlayer2.yPos = 85;
    spritePlayer2.id = 1;
    spritePlayer2.address = 0x0000;
    spritePlayer2.paletteNum = 1;
    spritePlayer2.currentAnim = &anim_evan_idle;

    loadSprite(&spritePlayer1);
    loadSprite(&spritePlayer2);
}

void tickSprite(SpriteFighter* sprite)
{
    Animation* anim = sprite->currentAnim;

    if (++anim->waitCounter >= anim->waitThreshold)
    {
        anim->waitCounter = 0;

        anim->frameCounter += anim->frameDirection;

        if (anim->reversable == 1)
        {
            if (anim->frameCounter >= anim->totalFrames)
            {
                anim->frameCounter -= 2;
                anim->frameDirection = -1;
            }
            else if (anim->frameCounter == 0)
            {
                anim->frameDirection = 1;
            }
        }
        else
        {
            if (anim->frameCounter >= anim->totalFrames)
            {
                anim->frameCounter = 0;
            }
        }

        loadSprite(sprite);
    }
}

void tickSprites()
{
    tickSprite(&spritePlayer1);
    tickSprite(&spritePlayer2);
}

void moveSprites()
{
}

void updateSprites()
{
}

/*
    sprite height/widths need to be aligned by 16 I think...

    using larger sprites would mean less for loop iterations
    consider this if speed becomes an issue

    For each 16x16 sprite, you have to calculate the graphics
    address correctly. Multiplying the index by two isn't enough,
    you also have to skip rows that are already included in the previous sprites
    For example, assume the line

    aaaaaaaaaa 
    bbbbbbbbbb
    cccccccccc
    dddddddddd

    you can render:   aa aa aa aa aa    as one row of sprites
                      bb bb bb bb bb
    but the next     
    line would be:    bb bb bb bb bb    which is incorrect, because we already showed
                      cc cc cc cc cc    bb
    
    in reality you    cc cc cc cc cc
    want to draw:     dd dd dd dd dd

*/
