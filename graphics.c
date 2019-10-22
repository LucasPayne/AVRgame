/* ENCE260 embedded systems project
 * Group 108
 * Drogo Shi, Lucas Payne
 * 
 * Definitions for the graphics module.
 */

#include "system.h"
#include "display.h"

#include "game.h"
#include "graphics.h"
#include "utility.h"

int BitmapBit(Bitmap *bm, int i, int j)
{
    return bm->bits[i + j * bm->vertical];
}


int bitmap_value(char *bits, int position)
{
    char bit = bits[position / 8];
    return !!((bit << (position % 8)) & 0x80);
}

void render_bitmap(Pos pos, int orientation, Bitmap *bm)
{
    if (orientation == North) {
        for (int i = 0; i < bm->horizontal; i++) {
            for (int j = 0; j < bm->vertical; j++) {
                if (bitmap_value(bm->bits, i + bm->horizontal * j)) {
                    render_screen_position (i + pos.x - bm->centre.x, j + pos.y - bm->centre.y);
                }
            }
        }
    } else if (orientation == South) {
        for (int i = 0; i < bm->horizontal; i++) {
            for (int j = 0; j < bm->vertical; j++) {
                int jt = bm->vertical - 1 - j;
                if (bitmap_value(bm->bits, i + bm->horizontal * jt)) {
                    render_screen_position (i + pos.x - bm->centre.x, j + pos.y - bm->centre.y);
                }
            }
        }
    } else if (orientation == West) {
        for (int i = 0; i < bm->vertical; i++) {
            for (int j = 0; j < bm->horizontal; j++) {
                int it = bm->vertical - 1 - i;
                if (bitmap_value(bm->bits, j + bm->horizontal * it)) {
                    render_screen_position (i + pos.x - bm->centre.x, j + pos.y - bm->centre.y);
                }
            }
        }
    } else if (orientation == East) {
        for (int i = 0; i < bm->vertical; i++) {
            for (int j = 0; j < bm->horizontal; j++) {
                int jt = bm->horizontal - 1 - j;
                if (bitmap_value(bm->bits, jt + bm->horizontal * i)) {
                    render_screen_position (i + pos.x - bm->centre.x, j + pos.y - bm->centre.y);
                }
            }
        }
    }
}

char screen_row_bits (int j)
{
    char bits = 0;
    for (int i = 0; i < SCREEN_X_RANGE; i++) {
        if (screen[i][j]) {
            bits |= 1 << (SCREEN_X_RANGE - i);
        }
    }
    return bits;
}

void render_row_bits (char bits, int j)
{
    for (int i = 0; i < SCREEN_X_RANGE; i++) {
        char bit = !!((bits << i) & 0x80);
        if (bit == 1) {
            render_screen_position (DISPLAY_X_RANGE - 1 - i, DISPLAY_Y_RANGE - 1 - j); //...
        }
    }
}


void display_screen()
{
    /* Maps part of the screen to the LED display. */
    for (int i = 0; i < SCREEN_X_RANGE; i++) {
        for (int j = 0; j < SCREEN_Y_RANGE; j++) {
            if (screen[i][j] && in_display_range(i, j)) {
                /* Map (i, j) in screen coordinates to display coordinates. */
                // display_pixel_set (1, 2, 1);
                display_pixel_set (DISPLAY_Y_RANGE - 1 - j, DISPLAY_X_RANGE - 1 - i, 1);
            }
        }
    }
}

void clear_screen()
{
    /* Wipe all to zeroes (should be done every frame). */
    for (int i = 0; i < SCREEN_X_RANGE; i++) {
        for (int j = 0; j < SCREEN_Y_RANGE; j++) {
            screen[i][j] = 0;
        }
    }
}


int in_screen_range(int x, int y)
{
    /* Returns whether or not a given position is on on the screen. */
    return 0 <= x && x < SCREEN_X_RANGE && 0 <= y && y < SCREEN_Y_RANGE;
}

int in_display_range(int x, int y)
{
    /* Returns whether or not a given position is on on the screen. */
    return 0 <= x && x < DISPLAY_X_RANGE && 0 <= y && y < DISPLAY_Y_RANGE;
}

void render_screen_position(int x, int y)
{
    if (in_screen_range(x, y)) {
        screen[x][y] = 1;
    }
}
