extends Node2D

export (int) var collision_bit = 10;

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	for cast in get_children():
		if cast.is_colliding():
			get_parent().set_collision_mask_bit(collision_bit,false);
			return;
