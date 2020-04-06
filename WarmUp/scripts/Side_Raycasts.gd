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
