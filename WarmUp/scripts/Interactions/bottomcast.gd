extends Node2D

export (int) var collision_bit = 10;
export (String) var disabler = "ui_down";
var disabled : bool = false;

func _input(event):
	if event.is_action_pressed(disabler):
		get_parent().set_collision_mask_bit(collision_bit, false);
		if !disabled:
			get_parent().velocity.y += 80;
		disabled = true;
	elif event.is_action_released(disabler):
		get_parent().set_collision_mask_bit(collision_bit, true);
		disabled = false;
	
	if event.is_action_pressed("ui_up"):
		get_parent().set_collision_mask_bit(collision_bit, false);
		disabled = true;
	elif event.is_action_released("ui_up"):
		get_parent().set_collision_mask_bit(collision_bit, true);
		disabled = false;

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if !disabled:
		for cast in get_children():
			if cast.is_colliding():
				get_parent().set_collision_mask_bit(collision_bit, true);
				return;
