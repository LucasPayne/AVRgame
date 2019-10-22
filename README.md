Lucas Payne, Drogo Shi

===============================================================
Contents:
    0: Manual for playing the game
    1: The game implementation
===============================================================

                DOGFIGHT IN SPACE
0: Manual
---------------------------------------------------------------
Connect both UCFKs to the same computer, and run
$make play.
This will start the game on both devices.
When a square image appears, the game can be started. Any player may press to start the game.
Align your UCFKs toward eachother: the screen is combined over both LED displays.

You are in a fight in space. You must destroy the opponent. Press or hold the push button to shoot.
Do not hit asteroids.
Move with the navswitch.


============= Implementation details incomplete ===============
>>>> Other player, symmetry
>>>> input receiving
>>>> destroying gameobjects

>>>> Clear display with no flicker or jitter
    May need to make frames slower
        maybe 120 displays per byte would work (83 now)
>>>> Add usercodes
>>> "Scenes"? Connecting, Game, Gameover, etc.

1: The game implementation:
------------------------------------
1.1: Avatar selection
1.2: Connection
1.3.1: The game loop
1.3.2: The gameobject system
1.4: Bitmap creation
1.5: Bitmap and animation structures
1.6: The rendering system
1.7: The animation system

------------------------------------
?? Connection and avatar selection:
    Each system starts in the master loop, but instead of a "world" gameobject,
    which contains the world_update() function, there is a "connection" gameobject,
    with connection_update(). In this way, all facilities can be used.

    Cycle through player bitmaps
    avatar1_bm
    avatar2_bm, etc.
    When pushed, animation is played (circle expand)
    and characters start being sent. 
    
    If any are received, then
    the game is started with this system as the master.

    If a character is received before pushing, then that system
    swaps to another connecting mode, where the player bitmaps
    can still be selected, but with push sending a single character,
    and the game starts with that system as slave.
------------------------------------
The gameobject system:  gameobjects.c
    The game logic is based off of "gameobjects".
    A pool of objects allows for instantiations with the macro
        create_game_object(GAMESTATE, NAME, X, Y),
    which expands to a call to 
        _create_game_object(GAMESTATE, &NAME_init, &NAME_update, X, Y).
    This function activates an object entry in the pool, and
    calls the given init function.
    
    For example, if asteroid_init() and asteroid_update() are defined,
        create_game_object(asteroid, 8, 6)
    will work and create an asteroid object at (8,6).

    Gameobjects have a bitmap pointer entry, which is used in the render phase of the game loop.
    This need not remain constant, and by default it is not set.

    The init function of the type of object can be given for example as
    void asteroid_init(Gamestate *g, Gameobject *o) {
        o->bitmap = &asteroid1_bm;
    }

    Gameobjects are defined in gameobjects.c

------------------------------------
The master loop:
    Once the master loop is entered, there is some initialization, and, importantly,
    a world gameobject is created.

    The loop consists of three major stages:
        Game logic (running the update functions of each gameobject in the pool)
        Rendering (mapping the bitmaps, if any, of each gameobject to the screen)
        Multiplexed
            Communication of slave display bits
            Receiving of slave inputs
            Input vector buildup
            LED display

    The loop terminates when there are no objects, implying the world is destroyed.

------------------------------------
Bitmap creation:    create_bitmaps.py, bitmap_definitions
    Bitmaps are defined in .bm text files, with _ off and X on, e.g.:
        _XX_
        XXOX
    Centres are defined by either C or O:
        C denotes it is a centre but is not active in the bitmap.
        O is a centre and is part of the bitmap.
        By default, centre is (0,0).
    create_bitmaps.py takes the directory structure
        bitmaps/
            player.bm
            animation_player_die/
                animation_player_die1.bm
                animation_player_die2.bm
                ...
            ...
    and generates a file of Bitmap and Animation structure definitions
        "bitmap_definitions"
    which can be included in the game files.

------------------------------------
Bitmap and animation structures:    graphics.h
    Bitmaps are just the texture data:
        int horizontal, vertical:
            dimensions of the bitmap
        Pos centre:
            when rendering a bitmap, this offsets the positions
        char bits[MAX_BITMAP_SIZE]:
            the bits are stored in the form
            _X_ -> 0b10101000,0b00000000,0b00000000,0b00000000
            XCX    centre: 1,0
    Animations consist of pointers to bitmaps:
        int speed:
            [UNIMPLEMENTED]
        int frames:
            the number of frames in this animation
        Bitmap *bitmaps[MAX_FRAMES]:
            Pointers to the bitmaps of frames of this animation
------------------------------------
        
The rendering system:   game.c, game_display.c, graphics.c
    The display.c module in Michael Hayes' AVR game library is used to drive the LEDs.

    Screen versus display:
        Screen: In-memory matrix
        9 -------
        8 -------
        7 -------
        6 -------       
        5 -------      LED matrix coordinates
        4[-------]     0[-------]
        3[-------]     1[-------]
        2[-------] --> 2[-------]
        1[-------]     3[-------]
        0[-------]     4[-------]
          0123456        6543210

    Every frame, the screen matrix is cleared.
    render():
        At the highest level of the rendering, a pass is made over the game state,
        making calls to render bitmaps from objects or active animations.
    render_bitmap(Pos pos, Bitmap *bm):
        This is the only graphics function which should be called by the game. This runs over
        the bitmap and activates portions of the screen.
        e.g.
            [asteroid1.bm]                     Bitmap asteroid1_bm = {
            [    __XX    ] definition creation     .horizontal = 4,
            [    _XXX    ] ------------------>     .vertical = 4,
            [    XXOX    ]                         .centre = {2, 1},
            [    XXX_    ]                         .bits = {0b11101111,0b01110011,0b00000000,0b00000000}
                                               };
            Pos point = {6, 4}; ...
            Pos point2 = {2, 0}; ...
            render_bitmap(point, &asteroid1_bm);
            render_bitmap(point2, &player_bm);
            Screen        
            9 -------      
            8 -------
            7 -------     
            6 ------X       
            5 -----XX         LED matrix coordinates
            4[----XXX]        0[----XXX]
            3[----XXX]        1[----XXX]
            2[-------] -...-> 2[-------]
            1[--X----]        3[--X----]
            0[-X-X---]        4[-X-X---]
              0123456           6543210
------------------------------------
The animation system:
    Animations are played with the function
        play_animation(Gamestate *g, Pos pos, Animation *animation),
    which is the only interface to the animation system that the game should use.

    This is given a pointer to the game state since it really is a game-level function.
    The gamestate stores active animations as "objects" in multiple arrays.
        [Gamestate structure...]
            Animation *animations[MAX_ANIMATIONS]:
                pointers to the animation structure, which contains frame information and bitmaps
            int animation_frames[MAX_ANIMATIONS];
                the current frame the active animation is at
            Pos animation_positions[MAX_ANIMATIONS];
                the position the animation is playing at
    play_animation() finds an empty index and stores the "object" with the current frame at 0 at the given position.
    It will simply not start an animation if the array is full.
    
    Rendering frames:
        render() has a step which makes a pass over the animation array, drawing the bitmap
        of the current frame, upping the frame, and checking it against the number of frames in the animation,
        removing it from the array (making the space available) if it is finished.


Syncing the screen:

The input system:
Receiving slave input:
    
        
        


display.c:
     display_pixel_set(col, row, val)

display.c, provided in the game library, provides
display_pixel_set(), which alters a display matrix
which is used by display.c's display_update().

At this level the display matrix is 5x7, and this is taken as
the "backend" to the game's display system, which allows
the rendering step of the loop to freely attempt drawing
on a 10x7 grid. Out-of-range draws are ignored, so the render
step can just make a pass over all objects, and clipping happens
automatically.

Finally, when the game display matrix is prepared,
it is synced up and each system displays their 5x7 half.




TODO:
Upload the bitmaps

Software used
Apt names:
    avr-libc
    gcc-avr:
        Has C compiler for the AVR architecture
    dfu-programmer:
        Flash onto firmware, used by makefile
    doxygen

