extends Node

export (int) var speed = 64;
export (bool) var enabled = false setget set_enabled;
export (bool) var from_input = true setget set_from_input;

var move_direction : int = 0 setget set_move;

enum lkdir {
	right = 1,
	left = -1
};

var look = lkdir.left;

signal start_moving();
signal direction_changed(direction);

onready var player = get_parent();

func set_move(value):
	value = sign(value);
	if value != 0:
		if move_direction == 0:
			emit_signal("start_moving");
		if look != value:
			look = value;
			emit_signal("direction_changed", look);
	move_direction = value;
	
func set_from_input(flag):
	set_process(flag);
	from_input = flag;
	
func _process(delta):
	set_move(- int(Input.is_action_pressed("ui_left")) + int(Input.is_action_pressed("ui_right")));

func set_enabled(flag):
	set_physics_process(flag);
	enabled = flag;

func _physics_process(delta):
	if move_direction != 0 && player.ground:
		player.call_deferred("animate", "_walk");
		player.get_node("Trail").emitting = true;
	player.velocity.x = lerp(player.velocity.x, speed * move_direction, 0.2);
