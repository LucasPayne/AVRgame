/* ENCE260 group 108
 * Drogo Shi, Lucas Payne
 * Header file for general utility functions and definitions.
 */
#ifndef UTILITY_HEADER
#define UTILITY_HEADER

enum Direction { // note: must be in line with other enums ...
    West,
    North,
    East,
    South
};
enum RelativeDirection {
    Left,
    Forward,
    Right,
    Backward
};

typedef struct Pos_s {
    char x;
    char y;
} Pos;
void set_pos(Pos *pos, int x, int y);

Pos shift_pos (Pos pos, int direction);

#endif
