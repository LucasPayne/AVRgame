void bullet_init(Gamestate *g, Gameobject *o)
{
    UNUSED (g);
    o->bitmap = &bullet_bm;
}
void bullet_update(Gamestate *g, Gameobject *o)
{
    if (o->lifetime == 6) {
        Gameobject *explode_effect = create_gameobject(g, effect, o->pos.x, o->pos.y);
        if (explode_effect) {
            effect_properties *effect_props = get_properties(effect, explode_effect);
            // effect_props->animation = &animation_player_explode;
            effect_props->animation = &animation_explode2;
        }
        destroy_gameobject(g, o);
        return;
    }
    
    o->pos = shifted_position(o, Forward);
}

