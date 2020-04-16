extends Particles2D



func set_flip(is_flip : bool):
	if is_flip:
		position.x = abs(position.x);
		process_material.direction.x = abs(process_material.direction.x);
	else:
		position.x = -abs(position.x);
		process_material.direction.x = -abs(process_material.direction.x);
	
	
	
