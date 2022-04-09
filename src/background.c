#include "data.h"
#include "background.h"

u16 mapTiles[32][32];
u16 mapLetters[32][32];

void loadMaps()
{
    u8 x = 0;
    u8 y = 0;
    for (x = 0; x < 32; x++)
    {
        for (y = 0; y < 32; y++)
        {
            mapTiles[y][x] = 0;
            mapLetters[y][x] = 0;
        }
    }

    //    WaitForVBlank();
    bgInitMapSet(0, ((u8*)&mapLetters), sizeof(mapLetters), SC_32x32, 0x2000);

    //   WaitForVBlank();
    bgInitMapSet(1, ((u8*)&mapTiles), sizeof(mapTiles), SC_32x32, 0x2000);
}

void loadTiles()
{
    bgInitTileSet(0, &gfx_letters_pic, &gfx_letters_pal, 0, gfx_letters_pic_size,
                  gfx_letters_pal_size, BG_4COLORS, 0x0000);

    WaitForVBlank();

    bgInitTileSet(0, &gfx_tiles_pic, &gfx_tiles_pal, 0, gfx_tiles_pic_size, gfx_tiles_pal_size,
                  BG_4COLORS, 0x1000);

    setMode(BG_MODE0, BG1_TSIZE8x8);

    WaitForVBlank();
    loadMaps();
    WaitForVBlank();
}