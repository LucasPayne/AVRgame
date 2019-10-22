void player1_init(Gamestate *g, Gameobject *o)
{
    player_init(g, o);
}
void player1_update(Gamestate *g, Gameobject *o)
{
    player_controls controls;
    controls.up_down = g->input_vector[UpButton].down;
    controls.down_down = g->input_vector[DownButton].down;
    controls.left_down = g->input_vector[LeftButton].down;
    controls.right_down = g->input_vector[RightButton].down;
    controls.push_down = g->input_vector[PushButton].down;
    
    player_update(g, o, &controls);
}

