#include <snes.h>

#include "data.h"

// IMPORTANT:
// Changing which pallette the BGs use is done via GFX compilation process
// * BGs always(?) use palette color #0 as the transparent color

void main()
{
    // Initialize SNES
    consoleInit();
    dmaClearVram();

    // Initialize game engine data
    WaitForVBlank();

    // load BG letters
    //bgInitTileSet(0, &gfx_letters_pic, &gfx_letters_pal, 0, gfx_letters_pic_size,
    //             gfx_letters_pal_size, BG_4COLORS, 0x0000);
    bgInitTileSet(0, &gfx_letters_pic, &gfx_letters_pal, 0, gfx_letters_pic_size,
                  gfx_letters_pal_size, BG_4COLORS, 0x0000);

    setScreenOn();

    while (1)
    {
        WaitForVBlank();
    }
}