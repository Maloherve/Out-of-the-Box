extends Node

export (int) var g = 800 setget set_gravity;
export (int) var limit_speed = 900;
export (bool) var enabled = true setget set_enabled;

onready var player = get_parent();

signal gravity_changed(value);

func set_gravity(value):
	emit_signal("gravity_changed", value);
	g = value;

func set_enabled(flag):
	set_physics_process(flag);
	enabled = flag;

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta):
	if player.velocity.y < limit_speed:
		player.velocity.y += g * delta;
