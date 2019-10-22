typedef struct player_properties_s {
    char speed;
    char health;
    char power_mode;
} player_properties;

void player_init(Gamestate *g, Gameobject *o)
{
    UNUSED (g);
    player_properties *props = get_properties(player, o);
    props->health = 5;
    o->bitmap = &player_bm;
}

void player_update(Gamestate *g, Gameobject *o, player_controls *controls)
{
    player_properties *props = get_properties(player, o);

    if (props->health <= 0) {
        Gameobject *explode_effect = create_gameobject(g, effect, o->pos.x, o->pos.y);
        if (explode_effect) {
            effect_properties *effect_props = get_properties(effect, explode_effect);
            effect_props->animation = &animation_player_explode;
        }
        destroy_gameobject(g, o);
        return;
    }

    if (controls->up_down) {
        o->pos = shifted_position(o, Forward);
    }
    if (controls->down_down) {
        o->pos = shifted_position(o, Backward);
    }
    if (controls->right_down) {
        o->pos = shifted_position(o, Right);
    }
    if (controls->left_down) {
        o->pos = shifted_position(o, Left);
    }
    if (controls->push_down) {
        // Gameobject *explode_effect = create_gameobject(g, effect, o->pos.x, o->pos.y);
        // effect_properties *effect_props = get_properties(effect, explode_effect);
        // effect_props->animation = &animation_powerup1;

        Gameobject *bullet = create_gameobject(g, bullet, o->pos.x, o->pos.y);
        if (bullet) {
            bullet->orientation = o->orientation;
        }
        // if (o->lifetime % 5 == 0) {
        //     Gameobject *ef = create_gameobject(g, effect, o->pos.x, o->pos.y);
        //     effect_properties *effect_props = get_properties(effect, ef);
        //     effect_props->animation = &animation_powerup1;;
        // }
    }
}


