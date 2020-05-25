extends Node

var jump_velocity : int = -388;
var jump_step : int = jump_velocity / 20;

onready var player = get_parent();

export (bool) var enabled = false setget set_enabled;
export (bool) var oneshot = false;
export (String) var action = "ui_up";

signal jumped;
signal end_jump;

func set_enabled(flag):
	set_process(true);
	enabled = flag;

func jump():
	player.animate("_jump");
	player.velocity.y += jump_velocity;
	emit_signal("jumped");
	if !oneshot:
		set_physics_process(true);
	enabled = false;
	
func break_jump(enable = false):
	emit_signal("end_jump");
	set_physics_process(false);
	enabled = enable;
		
func _process(delta):
	if !is_physics_processing() && Input.is_action_just_pressed(action):
		jump();

func _physics_process(delta):
	if Input.is_action_pressed(action):
		player.velocity.y += jump_step;
	else:
		break_jump();
