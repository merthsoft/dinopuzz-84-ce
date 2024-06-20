#include "theme.h"

Theme* Themes = (Theme[]){
    { "Modern", (const gfx_sprite_t**)tiles_modern_tiles_data, little_guy_modern, arrows_modern, icon_modern, 2, 1, 0, 3, 1 },
    { "CSE", (const gfx_sprite_t**)tiles_cse_tiles_data, little_guy_cse, arrows_cse, icon_cse, 2, 1, 0, 3, 1 },
    { "Retro", (const gfx_sprite_t**)tiles_oldschool_tiles_data, little_guy_oldschool, arrows_oldschool, icon_oldschool, 3, 1, 0x1a, 0x0c, 0x0c }
};

Theme* CurrentTheme = NULL;

uint8_t theme_menu_choice = 0;

void update_icon() {
    ti_var_t file = ti_OpenVar("DINOPUZZ", "r+", OS_TYPE_PROT_PRGM);
    if (!file) {return;}
    uint16_t size = ti_GetSize(file);
    uint16_t index = 0;
    uint8_t foundCount = 0;
    while (foundCount < 2 && index < size)
    {
        char buff;
        if (ti_Read(&buff, sizeof(char), 1, file) != 1)
            break;
        if (buff == 0x10)
            foundCount++;
        else
            foundCount = 0;
    }
    
    if (foundCount == 2)
    {
        ti_Write(CurrentTheme->icon->data, sizeof(uint8_t), 256, file);
    }
    ti_SetArchiveStatus(true, file);
    ti_Close(file);
}

enum state_transition theme_menu_update(sk_key_t key)
{    
    bool exit = false;
    switch (key)
    {
        case sk_Del:
            exit = true;
            break;
        case sk_Up:
            if (theme_menu_choice > 0)
                theme_menu_choice--;
            break;
        case sk_Down:
            if (theme_menu_choice < 3)
                theme_menu_choice++;
            break;
        case sk_2nd:
        case sk_Enter:
            switch (theme_menu_choice)
            {
                case 3:
                    exit = true;
                    break;
                default:
                    CurrentTheme = &Themes[theme_menu_choice];
                    update_icon();
                    exit = true;
                    break;
            }
    }
    
    return exit ? st_back : st_none;
}

void theme_menu_draw()
{
    Theme* theme = theme_menu_choice < 3 ? &Themes[theme_menu_choice] : CurrentTheme;
    gfx_FillScreen(theme->background_color);
    gfx_SetTextScale(3, 3);
    gfx_SetTextFGColor(theme->text_color);
    gfx_PrintStringXY("Themes", 2, 2);
    gfx_SetColor(theme->text_color);
    gfx_HorizLine_NoClip(2, 24, 100);
    gfx_HorizLine_NoClip(2, 25, 100);
    gfx_HorizLine_NoClip(2, 26, 100);
    uint8_t w = theme_menu_choice == 3 ? gfx_GetStringWidth("Quit") : gfx_GetStringWidth(theme->name);

    gfx_SetColor(theme->rectangle_color);
    gfx_FillRectangle(2, 28 + theme_menu_choice * 18, w + 2, 17);

    gfx_SetTextScale(2, 2);

    if (theme_menu_choice == 0)
        gfx_SetTextFGColor(theme->background_color);
    else
        gfx_SetTextFGColor(theme->text_color);
    gfx_PrintStringXY(Themes[0].name, 3, 29);

    if (theme_menu_choice == 1)
        gfx_SetTextFGColor(theme->background_color);
    else
        gfx_SetTextFGColor(theme->text_color);
    gfx_PrintStringXY(Themes[1].name, 3, 47);

    if (theme_menu_choice == 2)
        gfx_SetTextFGColor(theme->background_color);
    else
        gfx_SetTextFGColor(theme->text_color);
    gfx_PrintStringXY(Themes[2].name, 3, 65);

    gfx_SetTextFGColor(theme->text_color);
    gfx_PrintStringXY("Back", 2, 83);

    gfx_SetColor(theme->rectangle_color);
    gfx_FillRectangle(0, 195, GFX_LCD_WIDTH, 45);
    gfx_ScaledTransparentSprite_NoClip(theme->little_guy, 0, 195, theme->little_guy_scale, theme->little_guy_scale);

    uint8_t scale = theme->tile_scale; 
    for (uint8_t i = 0; i < 6; i++)
    {
        gfx_ScaledTransparentSprite_NoClip(theme->tileset[i], i * 32 + 50, 205, scale, scale);
    }
}