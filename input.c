/*
 * Definitions for the input module.
 */
#include "system.h"
#include "navswitch.h"
#include "input.h"


void check_input(InputInfo input_vector[])
{
    for (int i = 0; i < NUM_BUTTONS; i++) {
        input_vector[i].down = navswitch_down_p(i);
    }
}

void clear_input(InputInfo input_vector[])
{
    for (int i = 0; i < NUM_BUTTONS * 2; i++) {
        input_vector[i].down = 0;
        // input_vector[i].pushed = 0;
        // input_vector[i].released = 0;
    }
}
