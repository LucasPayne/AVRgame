
void asteroid2_init(Gamestate *g, Gameobject *o)
{
    UNUSED (g);
    o->bitmap = &asteroid2_bm;
}
void asteroid2_update(Gamestate *g, Gameobject *o)
{
    if (o->lifetime == 30) {
        destroy_gameobject(g, o);
    }
    o->pos = shift_pos(o->pos, East);
}
