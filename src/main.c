#include <snes.h>

#include "data.h"
#include "background.h"

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

    loadTiles();
    setScreenOn();

    while (1)
    {
        WaitForVBlank();
    }
}