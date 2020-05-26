extends Node

export (int) var speed = 140;
export (int) var friction = 400; 
export (bool) var enabled = false setget set_enabled;

export (bool) var input_enabled = true;

onready var player = get_parent();

var move : int = 0;

signal hold(flag);
signal on_ledge;

export (String) var up_action = "ui_up";
export (String) var down_action = "ui_down";

func set_enabled(flag):
	set_process(flag);
	enabled = flag;
	
func hold(flag):
	set_physics_process(flag);
	emit_signal("hold", flag);	

func stop_input():
	input_enabled = false;
	move = 0;
	
func _process(delta):
	if is_physics_processing():
		if input_enabled:
			move = - int(Input.is_action_pressed(up_action)) + int(Input.is_action_pressed(down_action));
		if !player.check_wall():
			hold(false);
	elif player.check_wall() && player.is_on_wall() && Input.is_action_pressed(up_action):
		hold(true); 

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta):
	if move != 0:
		player.velocity.y = lerp(player.velocity.y, speed * move, 0.2);
		if player.is_on_ledge() && Input.is_action_just_pressed(up_action):
			emit_signal("on_ledge");
			hold(false);
	else:
		player.velocity.y -= friction * delta;
		
