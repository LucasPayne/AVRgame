

void powerup1_init(Gamestate *g, Gameobject *o)
{
    UNUSED (g);
    o->bitmap = &powerup1_bm;
}
void powerup1_update(Gamestate *g, Gameobject *o)
{   
    if (o->lifetime % 10 == 0) {
        destroy_gameobject(g, o);
    }
    // o->pos = shift_position(o, Forward);
}
