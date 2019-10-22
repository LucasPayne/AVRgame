void player2_init(Gamestate *g, Gameobject *o)
{
    player_init(g, o);
}
void player2_update(Gamestate *g, Gameobject *o)
{
    player_controls controls;
    controls.up_down = g->input_vector[UpButton].down;
    controls.down_down = g->input_vector[DownButton].down;
    controls.left_down = g->input_vector[LeftButton].down;
    controls.right_down = g->input_vector[RightButton].down;
    controls.push_down = g->input_vector[PushButton].down;
    // controls.up_down = g->input_vector[OtherUpButton].down;
    // controls.down_down = g->input_vector[OtherDownButton].down;
    // controls.left_down = g->input_vector[OtherLeftButton].down;
    // controls.right_down = g->input_vector[OtherRightButton].down;
    // controls.push_down = g->input_vector[OtherPushButton].down;
    
    player_update(g, o, &controls);
}

