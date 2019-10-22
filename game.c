/* ENCE260 embedded systems project
 * Group 108
 * Drogo Shi, Lucas Payne
 */

/* AVR system Hayes' header files */
#include "display.h"
#include "ir_uart.h"
#include "ledmat.h"
#include "navswitch.h"
#include "pacer.h"
#include "pio.h"
#include "system.h"

/* Game-specific header files */
#include "gametasks.h"
#include "game.h"
#include "graphics.h"
#include "input.h"
#include "utility.h"

/* Generated data and code */
#include "bitmap_definitions"
#include "gameobject_definitions"


// Try to put this in another module
Gameobject *_create_gameobject 
                        (Gamestate *g,
                         void (*init_function) (Gamestate*, Gameobject*),
                         void (*update_function) (Gamestate*, Gameobject*),
                         int x, int y)
{
    for (int i = 0; i < MAX_GAMEOBJECTS; i++) {
        if (g->gameobject_on[i] == 0) {
            g->gameobject_on[i] = 1;
            g->gameobjects[i].init = init_function;
            g->gameobjects[i].update = update_function;
            g->gameobjects[i].orientation = North;
            Pos pos;
            pos.x = x;
            pos.y = y;
            g->gameobjects[i].pos = pos;
            g->gameobjects[i].lifetime = 0;

            g->gameobjects[i].visible = 1; // objects must hide themselves on init if they need to.
            
            g->gameobjects[i].init(g, &g->gameobjects[i]); // call the init function.

            return &g->gameobjects[i];
        }
    }
    return 0; // null pointer
}
void destroy_gameobject(Gamestate *g, Gameobject *o)
{
    g->gameobject_on[o - g->gameobjects] = 0;
}


int connect ()
{
    /* Standin connection function. */
    while (1) {
        // render_position_parts(1, 2);
        display_pixel_set(1, 1, 1);
        display_pixel_set(1, 2, 1);
        display_pixel_set(1, 3, 1);
        display_pixel_set(2, 3, 1);
        display_pixel_set(3, 3, 1);
        display_pixel_set(3, 2, 1);
        display_pixel_set(3, 1, 1);
        display_pixel_set(2, 1, 1);
        display_update ();
        
        if (ir_uart_read_ready_p ()) {
            return Slave;
        }
        // if (gamestate.input_vector[Push].down) {
        navswitch_update ();
        if (navswitch_down_p(NAVSWITCH_PUSH)) {
            ir_uart_putc ('S');
            return Game;
        }
        pacer_wait ();
    }
}


void game_update(Gamestate *g)
{
    /* Updates each gameobject. Called once a frame before render(). */
    for (int i = 0; i < MAX_GAMEOBJECTS; i++) {
        if (g->gameobject_on[i]) {
            g->gameobjects[i].update(g, &g->gameobjects[i]);
            g->gameobjects[i].lifetime ++;
        }
    }
}


void render (Gamestate *g)
{
    /* Prepares the game display matrix by rendering each gameobjects bitmap.
     * Called once a frame after game_update().
     */
    for (int i = 0; i < MAX_GAMEOBJECTS; i++) {
        if (g->gameobject_on[i] && g->gameobjects[i].visible) {
            render_bitmap(g->gameobjects[i].pos, g->gameobjects[i].orientation, g->gameobjects[i].bitmap);
        }
    }
}



/* Loops: master and slave loops, both start a task system. */
void game_loop()
{
    /* Game loop: The master loop.
     *     updates game objects, renders
     *     frame cycles: task multiplex:
     *          transmit display
     *          receive input
     *          build input
     */
    Gamestate gamestate = {0};
    Gamestate *g = &gamestate;
    create_gameobject(g, world, 0, 0);

    int num_tasks = 3;
    Task tasks[num_tasks];
    

    create_task(&tasks[0], &task_transmit_display, FRAME_STEPS/9, &gamestate);
    int receive_counter = 0;
    create_task(&tasks[1], &task_receive_input, FRAME_STEPS/25, &receive_counter);
    tasks[1].data2 = gamestate.input_vector;
    tasks[1].on = 1; // Turn this off for testing.
    create_task(&tasks[2], &task_build_input, FRAME_STEPS/10, &gamestate);

    while (1) {
        game_update (g);
        render (g);
        clear_input (g->input_vector);
        display_screen ();

        for (int i = 0; i < FRAME_STEPS; i++) {
            int any_task_done = task_multiplex_step (tasks, num_tasks, i);
            if (!any_task_done) {
                display_update ();
            }
        }
        reset_tasks(tasks, num_tasks);

        receive_counter = 0;

        clear_screen ();
        display_clear ();
    }
}



void slave_loop()
{
    /* Slave loop:
     *     waits for a signal to sync to the master loop
     *     frame cycles: task multiplex:
     *          build input
     *          transmit input
     *          receive display
     */
    int num_tasks = 3;
    Task tasks[num_tasks];
    
    // task_receive_display_state receive_display_data = {0};
    int received_count = 0;
    //create_task(&tasks[0], &task_receive_display, 1, &received_count);

    InputInfo input_vector[NUM_BUTTONS * 2] = {0};

    create_task(&tasks[0], &task_receive_display, FRAME_STEPS/20, &received_count);
    create_task(&tasks[1], &task_transmit_input, FRAME_STEPS/8, input_vector);
    create_task(&tasks[2], &task_slave_build_input, FRAME_STEPS/25, input_vector);


    while (1) {
        while (1) {
            if (ir_uart_read_ready_p ()) {
                char character = ir_uart_getc ();
                if (character == 'D') {
                    break;
                }
            }
            pacer_wait ();
            display_update ();
        }
        for (int i = 0; i < FRAME_STEPS; i++) {
            int any_task_done = task_multiplex_step (tasks, num_tasks, i);
            if (!any_task_done) {
                display_update ();
            }
        }

        reset_tasks(tasks, num_tasks);
        // Reset task state data
        received_count = 0;
        clear_input (input_vector);
    }
}


// Try to move these to another module.
Pos shifted_position(Gameobject *o, int direction)
{
    return shift_pos(o->pos, global_direction(o, direction));
}
int global_direction(Gameobject *o, int direction)
{
    /* Returns the global (North East South West) direction of the given direction
     * relative to the orientation of the gameobject.
     * Just a hardcoded case by case mapping.
     */
    if (o->orientation == North) {
        if (direction == Forward) {
            return North;
        } else if (direction == Left) {
            return West;
        } else if (direction == Right) {
            return East;
        } else if (direction == Backward) {
            return South;
        }
    } else if (o->orientation == West) {
        if (direction == Forward) {
            return West;
        } else if (direction == Left) {
            return South;
        } else if (direction == Right) {
            return North;
        } else if (direction == Backward) {
            return East;
        }
    } if (o->orientation == East) {
        if (direction == Forward) {
            return East;
        } else if (direction == Left) {
            return North;
        } else if (direction == Right) {
            return South;
        } else if (direction == Backward) {
            return West;
        }
    } if (o->orientation == South) {
        if (direction == Forward) {
            return South;
        } else if (direction == Left) {
            return East;
        } else if (direction == Right) {
            return West;
        } else if (direction == Backward) {
            return North;
        }
    }
    return North; //...
}


// Task definitions
void task_transmit_input (Task *task)
{
    InputInfo *input_vector = (InputInfo *) task->data;

    if (task->lifetime == 0) {
        ir_uart_putc ('I');
        // display_pixel_set (1, 1, 1); // >>>

        // navswitch_update ();
        // check_input (input_vector);
    } 
    else if (task->lifetime == 1) {
        char input_bits_1 = 0;
        for (int i = 0; i < 4; i++) {
            if (input_vector[i].down) {
                input_bits_1 |= 1 << (i * 2);
            }
            if (input_vector[i].press) {
                input_bits_1 |= 1 << (i * 2 + 1);
            }
        }
        ir_uart_putc (input_bits_1);
    }
    else if (task->lifetime == 2) {
        char input_bits_2 = 0;

        if (input_vector[4].down) {
            // display_pixel_set (1, 1, 1); // >>>
            // display_pixel_set (1, 1, 1); // >>>
            input_bits_2 |= 1;
        }
        if (input_vector[4].press) {
            input_bits_2 |= 1 << 1;
        }
        ir_uart_putc (input_bits_2);
    }
}
void task_receive_input(Task *task)
{
    int *receive_counter = (int *) task->data;
    InputInfo *input_vector = (InputInfo *) task->data2;

    if ((*receive_counter) <= 2 && ir_uart_read_ready_p ()) {
        char got_char = ir_uart_getc ();
        if (*receive_counter == 0) {
            if (got_char == 'I') {
                (*receive_counter) = 1;
            }
        } else if (*receive_counter == 1) {
            // display_pixel_set (1, 1, 1); // >>>
            for (int i = 0; i < 4; i++) {
                input_vector[NUM_BUTTONS + i].down = !!((got_char << (2*i)) & 0x80);
                input_vector[NUM_BUTTONS + i].press = !!((got_char << (2*i + 1)) & 0x80);
            }
            (*receive_counter) = 2;
        } else if (*receive_counter == 2) {
            input_vector[OtherPushButton].down = !!((got_char) & 0x80);
            input_vector[OtherPushButton].press = !!((got_char << 1) & 0x80);
            (*receive_counter) = 3;
        }
    }
}


void task_slave_build_input(Task *task)
{
    InputInfo *input_vector = (InputInfo *) task->data;
    navswitch_update ();
    check_input (input_vector);
}
void task_build_input(Task *task)
{
    Gamestate *g = (Gamestate *) task->data;
    navswitch_update ();
    check_input (g->input_vector);
}

void task_transmit_display(Task *task)
{
    if (task->lifetime == 0) {
        ir_uart_putc('D');
    } else if (task->lifetime == 1) {
        ir_uart_putc(screen_row_bits (5));
    } else if (task->lifetime == 2) {
        ir_uart_putc(screen_row_bits (6));
    } else if (task->lifetime == 3) {
        ir_uart_putc(screen_row_bits (7));
    } else if (task->lifetime == 4) {
        ir_uart_putc(screen_row_bits (8));
    } else if (task->lifetime == 5) {
        ir_uart_putc(screen_row_bits (9));
    }
}
void task_receive_display (Task *task)
{
    /* This is a more frequent task than the display transmission task.
     * After receiving the display sync byte, multiplex intermittent polls
     * for characters, up to 5, and align them to the LED matrix in their order.
     */

    // task_receive_display_state *state = (task_receive_display_state *) task->data;
    int *received_count = (int *) task->data;

    if ((*received_count) <= 5 && ir_uart_read_ready_p ()) {
        // if ((*received_count) == 5) { // >>>>>>>>>>>>>>>>>>>>>>>>
        //     display_pixel_set(1, 1, 1);
        // }
        char got_char = ir_uart_getc ();
        
        for (int i = 0; i < DISPLAY_X_RANGE; i++) {
            char bit = !!((got_char << i) & 0x80);
            display_pixel_set ((*received_count), i, bit);
        }
        
        *received_count += 1;
    }

    // display update this task
    display_update ();
}


int main(void)
{
    /* Declared in system.h: Initialise clock and disable watchdog timer.  */
    system_init ();
    /* Declared in ledmat.h: Initialise PIO pins to drive the LED matrix. */
    ledmat_init ();
    /* Declared in iruart.h */
    ir_uart_init ();
    /* Declared in pacer.h */
    pacer_init (LOOP_RATE);

    int mode = connect ();

    if (mode == Game) {
        game_loop();
    } else if (mode == Slave) {
        slave_loop();
    }
    return 0;
}
