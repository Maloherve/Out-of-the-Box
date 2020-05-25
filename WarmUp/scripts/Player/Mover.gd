extends Node

export (int) var speed = 64;
export (bool) var enabled = false setget set_enabled;

var move : int = 0 setget set_move;

enum lkdir {
	right,
	left
};

var look = lkdir.right;

signal direction_changed(direction);

onready var player = get_parent();

func set_move(value):
	if abs(value) > 1:
		value = sign(value);
	if move != value:
		
		if look == lkdir.right:
			look = lkdir.left;
		else:
			look = lkdir.right;
			
		emit_signal("direction_changed", value);
		move = value;

func set_enabled(flag):
	set_physics_process(flag);
	enabled = flag;

func _physics_process(delta):
	if move != 0:
		player.call_deferred("animate", "_move");
		player.velocity.x = lerp(player.velocity.x, speed * move, 0.2);
