#include <snes.h>
#include "data.h"
#include "backgroundScroll.h"

// Hardcoded Background engine constants
#define BG_NUMBER 1
#define BG_PAL_NUMBER 0
#define BG_SIZE_MODE SC_64x32
#define COLOR_MODE BG_16COLORS

// Background tiles take up 0x8000 (32x32) + 0x1000 (32x4)
// Tile space calculation = 8 * 4 * total_tiles
// 8 * 4 * (32 * 32) = 0x8000
// 8 * 4 * (32 * 4) = 0x1000
// Total VRAM = 0x10000 - 0x9000 = 0x7000 BYTES, not SHORTS

// WARNING!
// The Tile Address MUST BE ALONG A 0x1000 BOUNDARY. You can't do 0x1000 / 2,
// or else the SNES will not render the graphics correctly, even though
// they will end up at (0x1000 / 2) in memory...
#define TILE_ADDRESS 0x1000 // 0x7000 / 2
#define MAP_ADDRESS TILE_ADDRESS - (0x1000 / 2)

typedef struct
{
    u8* tileAddress;
    u8* paletteAddress;
    u16 tileSize;
    u16 paletteSize;

    u8* mapAddress;
    u16 mapSize;

} BGInfo;

typedef struct
{
    u8 bgCount;
    BGInfo bg[5];

} ScrollBG;

ScrollBG lake;
ScrollBG* currentScrollBG;

u16 columnAddressLookup[32 * 2];

void setScrollBackground()
{
    setMode(BG_MODE1, BG3_MODE1_PRORITY_HIGH);

    currentScrollBG = &lake;
    BGInfo* bg = &currentScrollBG->bg[0];
    bgInitTileSet(BG_NUMBER, bg->tileAddress, bg->paletteAddress, BG_PAL_NUMBER, bg->tileSize,
                  bg->paletteSize, COLOR_MODE, TILE_ADDRESS);
    bgInitMapSet(BG_NUMBER, bg->mapAddress, bg->mapSize, BG_SIZE_MODE, MAP_ADDRESS);

    // Disable all backgrounds except the relevant one in this file
    u8 i = 0;
    for (; i < 4; i++)
    {
        if (i == BG_NUMBER)
            bgSetEnable(i);
        else
            bgSetDisable(i);
    }

    setScreenOn(); // needed after enabling/disabling the backgrounds
    bgSetScroll(BG_NUMBER, 0, 0);

    WaitForVBlank();
}

void loadBackgroundAtPos(u16 x)
{
    u8 bgIndex = x / 256;
    u16 column_size = (8 * 4 * 30);

    bgSetScroll(BG_NUMBER, x, 0);

    u16 start_column = (x / 8) % 64;

    u8 col = 0;
    for (; col < 33; col++)
    {
        dmaCopyVram(currentScrollBG->bg[bgIndex].tileAddress + (col * column_size),
                    columnAddressLookup[(start_column + col) % 64], column_size);
        u8 test = 0;
        WaitForVBlank();
    }
}

/*
u8 step = 0;
#define colSize (8 * 4 * 30)
void scrollBGUpdate()
{
    return;
    if (step > 5)
        return;

    //dmaCopyVram(bufferTileMap, MAP_ADDRESS, sizeof(bufferTileMap));
    //dmaCopyVram(currentScrollBG->bg[1].tileAddress, 0x9800 / 2, (8 * 4 * 32));
    if (step == 0)
    {
        //step++;
        //return;
        //dmaCopyVram(currentScrollBG->bg[1].tileAddress, columnAddressLookup[50], colSize);

        // 50
        u8 i = 0;
        for (; i < 32; i++)
        {
            dmaCopyVram(currentScrollBG->bg[1].tileAddress + (i * colSize),
                        columnAddressLookup[(i + 50) % 64], colSize);
            WaitForVBlank();
        }
    }
    step++;
}*/

void initColumnLookup()
{
    u8 i = 0;
    // fill left side of columns
    for (; i < 32; i++)
    {
        u16 addr = TILE_ADDRESS + ((i * 8 * 4 * 30) / 2);
        columnAddressLookup[i] = addr;
        //consoleNocashMessage("%hu\n", addr);
    }

    // fill right side of columns
    i = 1;
    for (; i < 30; i++)
    {
        u16 addr = TILE_ADDRESS + ((i * 8 * 4 * 30) / 2);
        columnAddressLookup[i + 34] = addr;
        //consoleNocashMessage("%hu\n", addr);
    }

    // fill in padding column address
    columnAddressLookup[32] = 0x9800 / 2;
    columnAddressLookup[33] = 0x9800 / 2 + ((8 * 4 * 30) / 2);
    //for (i = 0; i < 64; i++)
    {
        //consoleNocashMessage("%hu %hu\n", (u16)i, columnAddressLookup[i]);
    }
}

void initScrollBackgrounds()
{
    lake.bgCount = 2;

    lake.bg[0].tileAddress = &gfx_lake_01_pic;
    lake.bg[0].paletteAddress = &gfx_lake_01_pal;
    lake.bg[0].mapAddress = &gfx_lake_01_map;
    lake.bg[0].tileSize = gfx_lake_01_pic_size;
    lake.bg[0].paletteSize = gfx_lake_01_pal_size;
    lake.bg[0].mapSize = gfx_lake_01_map_size;

    lake.bg[1].tileAddress = &gfx_lake_02_pic;
    lake.bg[1].paletteAddress = &gfx_lake_02_pal;
    lake.bg[1].mapAddress = &gfx_lake_02_map;
    lake.bg[1].tileSize = gfx_lake_02_pic_size;
    lake.bg[1].paletteSize = gfx_lake_02_pal_size;
    lake.bg[1].mapSize = gfx_lake_02_map_size;

    initColumnLookup();
}