#include "main.h"
#include "gfx/gfx.h"
#include "effects.h"
#include "menu.h"
#include "game.h"
#include "help.h"

#define copyright_length 24

int main(void)
{
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_ZeroScreen();
    srand(rtc_Time());

    memset(gfx_palette, 0, 512);
    int sizeof_base_palette = sizeof_global_palette / 2;

    int palette_size = sizeof_base_palette + copyright_length;

    uint16_t palette[sizeof_global_palette + copyright_length * 2];
    memcpy(palette, global_palette, sizeof_global_palette);

    for (int i = 0; i < copyright_length; i++)
        palette[i + sizeof_base_palette] = HsvToRgb(i * (255 / copyright_length), 255, 255);

    gfx_ZeroScreen();

    bool exit = false;
    bool fade_flag = true;
    enum game_state game_state = gs_main_menu;
    CurrentTheme = Themes;

    while (!exit)
    {
        int key = os_GetCSC();
        enum state_transition state_transition = st_none;

        switch (game_state)
        {
            case gs_main_menu:
                state_transition = main_menu_update(key);
                break;
            case gs_playing:
                state_transition = game_update(key);
                break;
            case gs_help:
                state_transition = help_update(key);
                break;
            case gs_theme:
                state_transition = theme_menu_update(key);
                break;
            default:
                state_transition = (key == sk_Del) ? st_back : st_none;
                break;
        }

        switch (state_transition)
        {
            case st_none:
                break;
            case st_back:
                switch (game_state)
                {
                    case gs_main_menu:
                        exit = true;
                        fade_flag = false;
                        break;
                    case gs_playing:
                    case gs_help:
                    case gs_theme:
                        fade_flag = true;
                        game_state = gs_main_menu;
                        break;
                }
                break;
            case st_fade_out:
                fade_flag = true;
                break;
            default:
                fade_flag = true;
                game_state = (enum game_state)state_transition;
                switch (game_state)
                {
                    case gs_playing:
                        game_init(4, 7, 2);
                        break;
                    default:
                        break;
                }
                game_state = (enum game_state)state_transition;
                break;
        }

        if (fade_flag)
            fade_out(palette, 0, palette_size); 

        gfx_ZeroScreen();

        switch (game_state)
        {
            case gs_main_menu:
                main_menu_draw();
                break;
            case gs_playing:
                game_draw();
                break;
            case gs_help:
                help_draw();
                break;
            case gs_theme:
                theme_menu_draw();
                break;
        }

        gfx_SwapDraw();

        if (fade_flag) 
        { 
            fade_in(palette, 0, palette_size); 
            fade_flag = false; 
        }
    }

    game_free();

    gfx_End();
    
    return 0;
}