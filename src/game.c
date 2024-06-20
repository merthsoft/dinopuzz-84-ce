
#include "main.h"
#include "game.h"
#include "theme.h"

#define MAX_STACK_SIZE 6
#define MAX_STACK_WIDTH 10

uint8_t num_stacks;
uint8_t stack_width;
uint8_t** stacks = 0;

uint8_t cursor = 0;

uint8_t num_incomming;
int8_t incomming_x[MAX_STACK_SIZE];
int8_t incomming_y[MAX_STACK_SIZE];
uint8_t incomming_tile[MAX_STACK_SIZE];

uint8_t standard_difficulty = 30;

uint8_t difficulty_timer = 0;

uint24_t score = 0;
uint8_t level = 1;

bool lose = false;

void draw_tile(uint8_t tile, uint8_t i, uint8_t j, struct Theme* theme)
{
    const gfx_sprite_t * sprite = theme->tileset[tile-1];
    uint8_t scale = theme->tile_scale; 
    gfx_ScaledSprite_NoClip(sprite, j * 32 + 32, i * 32, scale, scale);
}

void new_incomming()
{
    uint24_t y_cache = 0;

    for (int i = 0; i < num_incomming; i++)
    {
        incomming_x[i] = stack_width;
        do {
            int y = rand() % num_stacks;
            incomming_y[i] = (y_cache & (1 << y)) ? -1 : y;
        } while (incomming_y[i] == -1);
        
        y_cache |= 1 << incomming_y[i];
        incomming_tile[i] = (rand() % NUM_TILES) + 1;
    }
}

void game_init(uint8_t height, uint8_t width, uint8_t incomming)
{
    cursor = 0;
    difficulty_timer = 0;
    score = 0;
    level = 1;
    lose = false;

    if (!stacks)
    {
        stacks = malloc(sizeof(uint8_t*) * MAX_STACK_SIZE);
        for (int i = 0; i < num_stacks; i++)
            stacks[i] = malloc(sizeof(uint8_t) * MAX_STACK_WIDTH);
    }

    num_stacks = height;
    stack_width = width + 1;
    num_incomming = incomming;

    stacks = malloc(sizeof(uint8_t*) * num_stacks);
    for (int i = 0; i < num_stacks; i++)
    {
        stacks[i] = malloc(sizeof(uint8_t) * stack_width);
        memset(stacks[i], 0, stack_width);
    }

    memset(incomming_x, 0, num_incomming);
    memset(incomming_y, 0, num_incomming);
    memset(incomming_tile, 0, num_incomming);

    new_incomming();
}

void game_free() 
{
    if (stacks)
    {
        for (int i = 0; i < num_stacks; i++)
            free(stacks[i]);
        free(stacks);
        stacks = 0;
    }
}

void add_score(uint8_t points)
{
    score += points;
    level = score / 50 + 1;
}

enum state_transition game_update(sk_key_t key)
{
    if (lose)
        return key == sk_Del ? st_back : st_none;

    switch (key)
    {
        case sk_Del:
            return st_back;
            break;
        case sk_Up:
            if (cursor > 0)
                cursor--;
            break;
        case sk_Down:
            if (cursor < num_stacks - 2)
                cursor++;
            break;
        case sk_2nd:
            for (int i = 0; i < num_incomming; i++)
            {
                uint8_t tile = incomming_tile[i];
                if (tile == 0)
                    continue;

                int8_t in_x = incomming_x[i];
                if (in_x == stack_width)
                    continue;
                int8_t in_y = incomming_y[i];

                if (in_y == cursor)
                {
                    uint8_t under = stacks[in_y + 1][in_x];
                    if (under)
                        incomming_y[i] = cursor + 1;
                    break;
                }
                else if (in_y == cursor + 1)
                {
                    uint8_t over = stacks[in_y - 1][in_x];
                    
                    if (over)
                        incomming_y[i] = cursor;
                    break;
                }
            }
            
            uint8_t** stacks_pointers = (uint8_t **)stacks;
            uint8_t* stack = stacks[cursor];
            stacks_pointers[cursor] = stacks[cursor + 1];
            stacks_pointers[cursor+1] = stack;
            break;
    }

    uint16_t mod = standard_difficulty - level;
    difficulty_timer = (difficulty_timer + 1) % (mod > 0 ? mod : 1);
    
    if (difficulty_timer == 0 || key == sk_Left)
    {
        difficulty_timer = 0;
        bool all_done = true;
        for (int i = 0; i < num_incomming; i++)
        {
            uint8_t tile = incomming_tile[i];
            if (tile == 0)
                continue;
            all_done = false;
            incomming_x[i]--;
            if (incomming_x[i] == -1 || stacks[incomming_y[i]][incomming_x[i]])
            {
                int8_t x = incomming_x[i] + 1;
                int8_t y = incomming_y[i];
                if (tile == CLOSE_SHELL)
                {
                    for (int i = x; i >= 0; i--)
                    {
                        if (stacks[y][i] == OPEN_SHELL)
                        {
                            for (int j = i; j <= x; j++)
                                stacks[y][j] = 0;
                            add_score(60 + 10 * (x - i + 1));
                            break;
                        }
                    }
                } else if (x > 0 && stacks[y][x-1] == tile)
                {
                    stacks[y][x] = 0;
                    stacks[y][x-1] = 0;
                    add_score(5);
                } else if (x == stack_width)
                    lose = true;
                else
                    stacks[y][x] = tile;
                incomming_tile[i] = 0;
            }
        }

        if (all_done)
            new_incomming();
    }
    return st_none;
}

void game_draw()
{
    gfx_FillScreen(CurrentTheme->background_color);

    gfx_ScaledSprite_NoClip(CurrentTheme->arrows, 0, 32 * cursor, 2, 2);

    for (int i = 0; i < num_stacks; i++)
    {
        for (int j = 0; j < stack_width; j++)
        {
            uint8_t tile = stacks[i][j];
            if (tile == 0)
                break;

            draw_tile(tile, i, j, CurrentTheme);
        }
    }

    if (num_stacks < 6)
        gfx_ScaledSprite_NoClip(CurrentTheme->tileset[NUM_TILES], 0, 160, CurrentTheme->tile_scale, CurrentTheme->tile_scale);
    if (num_stacks < 5)
        gfx_ScaledSprite_NoClip(CurrentTheme->tileset[NUM_TILES], 0, 128, CurrentTheme->tile_scale, CurrentTheme->tile_scale);

    for (int i = 0; i < num_incomming; i++)
    {
        uint8_t tile = incomming_tile[i];
        if (tile == 0)
            continue;
        
        draw_tile(tile, incomming_y[i], incomming_x[i], CurrentTheme);
    }

    gfx_SetColor(CurrentTheme->text_color);
    gfx_VertLine_NoClip(286, 0, 200);
    gfx_VertLine_NoClip(287, 0, 200);

    gfx_SetColor(CurrentTheme->rectangle_color);
    gfx_FillRectangle_NoClip(0, 195, GFX_LCD_WIDTH, 45);
    gfx_ScaledTransparentSprite_NoClip(CurrentTheme->little_guy, 0, 195, CurrentTheme->little_guy_scale, CurrentTheme->little_guy_scale);

    gfx_SetTextScale(2, 2);
    gfx_SetTextFGColor(CurrentTheme->text_color);
    char buffer[50];
    sprintf(buffer, "Level: %i", level);
    gfx_PrintStringXY(buffer, 50, 202);
    sprintf(buffer, "Score: %i", score);
    gfx_PrintStringXY(buffer, 50, 220);

    if (lose)
    {
        gfx_SetTextFGColor(CurrentTheme->text_color);
        gfx_SetTextScale(2, 3);
        int w = gfx_GetStringWidth("You lose!");

        gfx_SetColor(CurrentTheme->rectangle_color);
        gfx_FillRectangle_NoClip((GFX_LCD_WIDTH - w - 8) / 2, (GFX_LCD_HEIGHT - 24) / 2 - 4, w + 8, 46);
        gfx_PrintStringXY("You lose!", (GFX_LCD_WIDTH - w) / 2, (GFX_LCD_HEIGHT - 24) / 2);
        gfx_SetTextScale(1, 1);
        gfx_PrintStringXY("Press del", (GFX_LCD_WIDTH - w) / 2, (GFX_LCD_HEIGHT - 24) / 2 + 26);
    }
}