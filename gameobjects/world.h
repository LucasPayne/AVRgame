void world_init(Gamestate *g, Gameobject *o)
{
    o->visible = 0;
    Gameobject *player1 = create_gameobject(g, player1, 3, 1);
    player1->orientation = North;
    Gameobject *player2 = create_gameobject(g, player2, 3, 8);
    player2->orientation = South;
}
void world_update(Gamestate *g, Gameobject *o)
{
    // UNUSED (g); UNUSED (o);


    if (o->lifetime % 101 == 0) {
        create_gameobject(g, asteroid, 10, 4);
    }
    if (o->lifetime % 73 == 0) {
        create_gameobject(g, asteroid2, -1, 7);
    }

    // if (o->lifetime % 101 == 0) {
    //     create_gameobject(g, asteroid, 10, 5);
    // }
    // if (o->lifetime % 43 == 0) {
    //     create_gameobject(g, asteroid2, -1, 3);
    // }
    // if (o->lifetime % 47 == 0) {
    //     create_gameobject(g, asteroid2, -1, 7);
    // }
}

