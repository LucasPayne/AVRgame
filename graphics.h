/* ENCE260 embedded systems project
 * Group 108
 * Drogo Shi, Lucas Payne
 * 
 * Header file for the graphics module.
 */

#ifndef GRAPHICS_HEADER_DEFINED
#define GRAPHICS_HEADER_DEFINED

#define DISPLAY_X_RANGE 7
#define DISPLAY_Y_RANGE 5
#define SCREEN_X_RANGE 7
#define SCREEN_Y_RANGE 10
#define MAX_BITMAP_SIZE 4

#include "utility.h"

typedef struct Bitmap_s {
    char horizontal;
    char vertical;
    Pos centre;
    char bits[MAX_BITMAP_SIZE];
} Bitmap;

#define MAX_FRAMES 5
typedef struct Animation_s {
    char speed;
    char frames;
    Bitmap *bitmaps[MAX_FRAMES];
} Animation;

char screen[SCREEN_X_RANGE][SCREEN_Y_RANGE];

void display_at_pos(Pos v);
void flash_at_pos(Pos v);
void display_screen();
void clear_screen();
int in_screen_range(int x, int y);
int in_display_range(int x, int y);
void render_screen_position(int x, int y);
void render_row_bits (char bits, int j);
char screen_row_bits (int j);

int BitmapBit(Bitmap *bm, int i, int j);
void render_bitmap(Pos pos, int orientation, Bitmap *bm);

#endif
