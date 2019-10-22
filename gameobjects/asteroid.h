void asteroid_init(Gamestate *g, Gameobject *o)
{
    UNUSED (g);
    o->bitmap = &asteroid1_bm;
}
void asteroid_update(Gamestate *g, Gameobject *o)
{
    if (o->lifetime == 50) {
        destroy_gameobject(g, o);
    }
    if (o->lifetime % 2 == 0) {
        o->pos = shift_pos(o->pos, West);
    }
}
