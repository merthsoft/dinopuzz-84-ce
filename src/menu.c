#include "main.h"
#include "menu.h"

uint8_t menu_choice = 0;

uint8_t copyright_shimmer = 0;
const char copyright[] = "2022 Merthsoft Creations";

#define copyright_length 24

enum state_transition main_menu_update(sk_key_t key)
{    
    bool exit = false;
    switch (key)
    {
        case sk_Del:
            exit = true;
        case sk_Up:
            break;
            if (menu_choice > 0)
                menu_choice--;
            break;
        case sk_Down:
            if (menu_choice < 3)
                menu_choice++;
            break;
        case sk_2nd:
        case sk_Enter:
            switch (menu_choice)
            {
                case 0:
                    return st_playing;
                case 1:
                    return st_help;
                case 2:
                    return st_theme;
                case 3:
                    exit = true;
                    break;
            }
    }
    
    return exit ? st_back : st_none;
}

void main_menu_draw()
{
    gfx_ScaledSprite_NoClip(palm_leaf, 0, 0, 2, 2);
    gfx_ScaledSprite_NoClip(palm_leaf_right, 250, 0, 2, 2);
    gfx_ScaledSprite_NoClip(egg_shadow, 4, 202, 2, 2);
    gfx_ScaledTransparentSprite_NoClip(egg, 6, 94, 2, 2);
    gfx_ScaledTransparentSprite_NoClip(logo, 62, 24, 2, 2);

    gfx_SetTextScale(2, 2);
    gfx_SetTextFGColor(1);
    uint8_t w = menu_choice == 2 ? gfx_GetStringWidth("Theme") : gfx_GetStringWidth("Play");

    uint8_t cursor_x = 188;
    uint8_t cursor_y = 147 + menu_choice * 18;

    gfx_SetColor(3);
    gfx_FillRectangle(cursor_x, cursor_y, w + 2, 18);

    gfx_PrintStringXY("Play", 190, 149);
    gfx_PrintStringXY("Help", 190, 167);
    gfx_PrintStringXY("Theme", 190, 185);
    gfx_PrintStringXY("Quit", 190, 204);

    gfx_SetTextScale(1, 1);
    w = gfx_GetStringWidth(copyright);

    for (uint8_t i = 0; i < copyright_length; i++)
    {
        int palette_index = (i + copyright_shimmer) % copyright_length;
        gfx_SetTextFGColor(palette_index + sizeof_global_palette / 2);
        gfx_SetTextXY(GFX_LCD_WIDTH / 2 - w / 2 + 8 * i, 2);
        gfx_PrintChar(copyright[i]);
    }

    if (copyright_shimmer <= copyright_length)
        copyright_shimmer++;
}