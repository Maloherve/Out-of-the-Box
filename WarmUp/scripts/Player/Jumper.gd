extends Node

export (int) var jump_velocity = -100;
var jump_step : int = jump_velocity / 5;
var jump_count : int = 0;

onready var player = get_parent();

export (bool) var enabled = true setget set_enabled;
export (bool) var oneshot = false;
export (int) var hold_ticks = 12;
export (String) var action = "ui_up";

signal jumped;
signal end_jump;

func _ready():
	player.connect("landed", self, "_on_Player_landed");

func set_enabled(flag):
	set_process(flag);
	enabled = flag;
	
func _on_Player_landed():
	break_jump(true);

func jump():
	player.velocity.y += jump_velocity;
	emit_signal("jumped");
	$sound.play();
	if !oneshot:
		set_physics_process(true);
	set_enabled(false);
	
func break_jump(enable = false):
	emit_signal("end_jump");
	set_physics_process(false);
	jump_count = 0;
	set_enabled(enable);
		
func _process(delta):
	if !is_physics_processing() && Input.is_action_just_pressed(action):
		jump();

func _physics_process(delta):
	var hold = Input.is_action_pressed(action);
	if hold:
		player.velocity.y += jump_step;
		jump_count += 1;
	if !hold || jump_count >= hold_ticks:
		break_jump();
