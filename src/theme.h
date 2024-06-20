#ifndef __THEME_H_
#define __THEME_H_

#include "main.h"
#include <graphx.h>
#include "gfx/gfx.h"
#include <ti/getcsc.h>

typedef struct Theme {
    const char* name;
    const gfx_sprite_t** tileset;
    const gfx_sprite_t* little_guy;
    const gfx_sprite_t* arrows;
    const gfx_sprite_t* icon;
    uint8_t tile_scale;
    uint8_t little_guy_scale;
    uint8_t background_color;
    uint8_t rectangle_color;
    uint8_t text_color;
} Theme;

#define NUM_THEMES 3
extern Theme* Themes;
extern Theme* CurrentTheme;

enum state_transition theme_menu_update(sk_key_t key);
void theme_menu_draw();

#endif