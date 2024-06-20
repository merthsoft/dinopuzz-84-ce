#ifndef __HELP_H_
#define __HELP_H_

#include "main.h"

void help_draw()
{
    gfx_ZeroScreen();
    gfx_SetTextFGColor(8);
    gfx_SetTextScale(3, 3);
    gfx_PrintStringXY("Dino Puzzle", 2, 2);
    int w = gfx_GetStringWidth("Dino Puzzle");
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY("v0.2", w + 6, 8);
    gfx_SetTextScale(1, 1);
    gfx_SetTextFGColor(12);
    gfx_PrintStringXY("Merthsoft Creations 2022", 2, 26);

    gfx_SetTextFGColor(1);
    
    gfx_SetTextScale(1, 1);
    gfx_PrintStringXY("Programming - Shaun McFall", 2, GFX_LCD_HEIGHT - 28);
    gfx_PrintStringXY("Original game - Brandon Sterner", 2, GFX_LCD_HEIGHT - 19);
    gfx_PrintStringXY("Graphics - Above, LD Studios, Aleksandr Makarov", 2, GFX_LCD_HEIGHT - 10);
}

enum state_transition help_update(sk_key_t key)
{
    return key == sk_Del ? st_back : st_none;
}

#endif