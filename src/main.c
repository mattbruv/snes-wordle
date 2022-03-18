#include <snes.h>

#include "backgroundScroll.h"
#include "vblank.h"
#include "data.h"
#include "sprite.h"

// IMPORTANT:
// Changing which pallette the BGs use is done via GFX compilation process
// * BGs always(?) use palette color #0 as the transparent color

void main()
{
    // Initialize SNES
    consoleInit();
    dmaClearVram();

    // Initialize game engine data
    initScrollBackgrounds();

    WaitForVBlank();

    // Set up test game state
    setScrollBackground();

    setScreenOn();

    initSprites();

    while (1)
    {
        // scrollBGUpdate();
        tickSprites();
        WaitForVBlank();
    }
}