/*
 * Header file for the input module.
 */
#ifndef INPUT_HEADER
#define INPUT_HEADER

#define NUM_BUTTONS 5

/* Usage of g for the Gamestate pointer is standard. Macros for convenience. */
#define UP_BUTTON_DOWN g->input_vector[UpButton].down
#define DOWN_BUTTON_DOWN g->input_vector[DownButton].down
#define LEFT_BUTTON_DOWN g->input_vector[LeftButton].down
#define RIGHT_BUTTON_DOWN g->input_vector[RightButton].down
#define PUSH_BUTTON_DOWN g->input_vector[PushButton].down

enum Button {
    RightButton,
    DownButton,
    LeftButton,
    UpButton,
    PushButton,
    OtherRightButton,
    OtherDownButton,
    OtherLeftButton,
    OtherUpButton,
    OtherPushButton
};

typedef struct InputInfo_s {
    char down;
    char press;
} InputInfo;

void check_input(InputInfo input_vector[]);
void clear_input(InputInfo input_vector[]);

#endif
