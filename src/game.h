#ifndef __GAME_H_
#define __GAME_H_

#include "main.h"
#include "theme.h"

#define NUM_TILES 6

#define OPEN_SHELL 5
#define CLOSE_SHELL 6

void game_init(uint8_t height, uint8_t width, uint8_t incomming);

enum state_transition game_update(sk_key_t key);
void game_draw();

void game_free();

#endif