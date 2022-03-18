#ifndef SPRITE_H
#define SPRITE_H

#include <snes.h>

typedef struct AnimFrame
{
    u8* picAddr;
    u8* palAddr;
    u16 picSize;
    u16 palSize;

    u8 rowCount;
    u8 colCount;

    u8 centerX;
    u8 centerY;
} AnimFrame;

typedef struct Animation
{
    u8 frameCounter;
    u8 totalFrames;

    u8 waitCounter;
    u8 waitThreshold;

    u8 reversable;
    s8 frameDirection;

    AnimFrame* frames;
} Animation;

void initSprites();
void updateSprites();
void tickSprites();

#endif