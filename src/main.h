#ifndef __MAIN_H_
#define __MAIN_H_

#include <graphx.h>
#include <debug.h>
#include <ti/getcsc.h>
#include <debug.h>
#include <sys/timers.h>
#include <string.h>
#include <fileioc.h>
#include <sys/rtc.h>

#include "gfx/gfx.h"

enum game_state { gs_main_menu = 1, gs_playing, gs_help, gs_theme };

enum state_transition
{
    st_fade_out = -2,
    st_back = -1,
    st_none = 0,
    st_main_menu = gs_main_menu,
    st_playing = gs_playing,
    st_help = gs_help,
    st_theme = gs_theme
};

#endif