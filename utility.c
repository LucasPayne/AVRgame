/* Utility function definitions, and shared global variables.
 */
#include "utility.h"

void set_pos(Pos *pos, int x, int y) {
    pos->x = x;
    pos->y = y;
}

Pos dir_vector[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    // West,
    // North,
    // East,
    // South

Pos displaced_pos (Pos pos, Pos displacement)
{
    Pos new_pos;
    new_pos.x = pos.x + displacement.x;
    new_pos.y = pos.y + displacement.y;
    return new_pos;
}

Pos shift_pos (Pos pos, int direction)
{
    return displaced_pos(pos, dir_vector[direction]);
}
