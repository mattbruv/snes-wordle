#include <snes.h>

#include "background.h"
#include "data.h"

// IMPORTANT:
// Changing which pallette the BGs use is done via GFX compilation process
// * BGs always(?) use palette color #0 as the transparent color

void initBackground()
{
    consoleNocashMessage("Initializing Background..\n");

    setMode(BG_MODE1, BG3_MODE1_PRORITY_HIGH);

    bgSetDisable(0);
    bgSetEnable(1);
    bgSetDisable(2);
    bgSetDisable(3);

    bgDemo();
    setScreenOn();
    WaitForVBlank();
}

/*
    Something is definitely fucky with the 4k boundaries.
    4k doesn't mean 0x4000. It's actually 4096.
    but the pvsneslib documentation is so piss poor you're probably
    going to have to understand how the tilemap is laid out in the SNES
    instead of just relying on the library to do what it says it will do
    because it doesn't ever seem to line things up where I tell it to

*/

/*
    for some reason, BSNES shows twice as much VRAM.
    the bsnes tile viewer shows tile addresses up to 0xffe0
    whereas NO$SNS only shows 0x7ff0 tile addresses
    an address in BSNES is (Address * 2)
*/

void bgDemo()
{
    //bgInitTileSet(1, &gfx_pee_pic, &gfx_pee_pal, 1, gfx_pee_pic_size, gfx_pee_pal_size, BG_16COLORS,
    //             0x1000);

    //bgInitMapSet(1, &gfx_pee_map, gfx_pee_map_size, SC_64x32, 0x0000);
}

/*
void bgMichael()
{
    bgInitTileSet(1, &gfx_pee_pic, &gfx_pee_pal, 1, gfx_pee_pic_size, gfx_pee_pal_size, BG_16COLORS,
                  0x4000);
    bgInitMapSet(1, &gfx_pee_map, gfx_pee_map_size, SC_64x32, 0x2000);
}
*/