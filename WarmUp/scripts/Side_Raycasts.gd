extends Node2D


# Flip the direction of the Raycasts
func set_flip(flip):
	if flip:
		for raycast in get_children():
			raycast.cast_to[0] = abs(raycast.cast_to[0]) * -1;
			raycast.cast_to[1] = abs(raycast.cast_to[1]) * -1;
	else:
		for raycast in get_children():
			raycast.cast_to[0] = abs(raycast.cast_to[0]);
			raycast.cast_to[1] = abs(raycast.cast_to[1]);

func check():
	for raycast in get_children():
		if raycast.is_colliding():
			return true;
	return false;

func is_front_colliding():
	return $Top_Side.is_colliding();
	
func front_collisions():
	var out = [];
	for raycast in get_children():
		if raycast.is_colliding():
			out.append(raycast.get_collider());
	return out;

func is_on_ledge():
	return $Bottom_Side.is_colliding() && ! $Top_Side.is_colliding();
