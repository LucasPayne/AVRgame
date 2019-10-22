
// BEGIN_PROPERTIES
typedef struct effect_properties_s {
    Animation *animation;
} effect_properties;
// END_PROPERTIES

void effect_init(Gamestate *g, Gameobject *o)
{
    UNUSED(g); UNUSED(o);
}
void effect_update(Gamestate *g, Gameobject *o)
{
    struct effect_properties_s *props = get_properties(effect, o);
    if (o->lifetime >= props->animation->frames) {
        destroy_gameobject(g, o);
        return;
    }
    o->bitmap = props->animation->bitmaps[o->lifetime];
}

