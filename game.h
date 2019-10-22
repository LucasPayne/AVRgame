/* ENCE260 embedded systems project
 * Group 108
 * Drogo Shi, Lucas Payne
 * 
 * Header file for the fundamental game functions
 * and structures.
 */

#ifndef GAME_HEADER_DEFINED
#define GAME_HEADER_DEFINED

#define LOOP_RATE 2500
#define FRAME_STEPS 500

#define UNUSED(LVALUE)\
    LVALUE = LVALUE

#define MAX_GAMEOBJECTS 15
#define PROPERTIES_SIZE 4 // Number of chars of space for gameobject properties

#define create_gameobject(GAMESTATE,NAME,X,Y)\
    _create_gameobject((GAMESTATE),NAME ## _init,NAME ## _update,(X),(Y))

#define get_properties(NAME,GAMEOBJECT)\
    (NAME ## _properties *) &(GAMEOBJECT ->properties)

#include "graphics.h"
#include "input.h"
#include "utility.h"
#include "gametasks.h"

enum Mode {
    Connecting,
    Game,
    Slave
};

typedef struct Gamestate_s Gamestate;
typedef struct Gameobject_s Gameobject;

typedef struct Gameobject_s {
    void (*init) (Gamestate*, Gameobject*);
    void (*update) (Gamestate*, Gameobject*);
    char orientation; // North, East, South, West
    char visible;
    short int lifetime;
    Bitmap *bitmap;
    Pos pos;
    char properties[PROPERTIES_SIZE];
} Gameobject;

typedef struct Gamestate_s {
    InputInfo input_vector[NUM_BUTTONS * 2];
    Gameobject gameobjects[MAX_GAMEOBJECTS];
    char gameobject_on[MAX_GAMEOBJECTS];
} Gamestate;


typedef struct player_controls_s {
    char up_down;
    char down_down;
    char left_down;
    char right_down;
    char push_down;
} player_controls;


Gameobject *_create_gameobject 
                        (Gamestate *g,
                         void (*init_function) (Gamestate*, Gameobject*),
                         void (*update_function) (Gamestate*, Gameobject*),
                         int x, int y);
void destroy_gameobject(Gamestate *g, Gameobject *o);


void render (Gamestate *g);
int connect ();
void game_loop();
void slave_display ();
void slave_loop();
void play_animation(Gamestate *g, Pos pos, Animation *animation);


// try to move to another module
Pos shifted_position(Gameobject *o, int direction);
int global_direction(Gameobject *o, int direction);

// Task declarations
void task_transmit_display(Task *task);
void task_receive_display (Task *task);

void task_slave_build_input(Task *task);
void task_build_input(Task *task);
void task_transmit_input(Task *task);
void task_receive_input (Task *task);


#endif
