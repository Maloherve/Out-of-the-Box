extends Node

export (int) var speed = 140;
export (int) var friction = 400; 
export (bool) var enabled = false setget set_enabled;

export (bool) var input_enabled = true;

onready var player = get_parent();

var move : int = 0;
var holding : bool = false;

signal hold(flag);
signal on_ledge;

export (String) var up_action = "ui_up";
export (String) var down_action = "ui_down";

func _ready():
	player.connect("landed", self, "_on_land")

func set_enabled(flag):
	set_process(flag);
	enabled = flag;
	
func hold(flag):
	if flag != holding:
		set_physics_process(flag);
		emit_signal("hold", flag);
		if flag && player.is_on_floor():
			player.velocity.y -= 300;	
		holding = flag;
	
func _on_land():
	if enabled && holding:
		hold(false);

func stop_input():
	input_enabled = false;
	move = 0;
	
func _process(delta):
	if !enabled: # set_process is bugged
		return;
	if holding:
		if input_enabled:
			move = - int(Input.is_action_pressed(up_action)) + int(Input.is_action_pressed(down_action));
		if !player.check_wall():
			hold(false);
	elif player.check_wall() && player.is_on_wall() && Input.is_action_pressed(up_action):
		hold(true); 

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta):
	if holding:
		if move != 0:
			player.velocity.y = lerp(player.velocity.y, speed * move, 0.2);
			if player.is_on_ledge() && Input.is_action_pressed(up_action):
				emit_signal("on_ledge");
				player.velocity.y -= speed / 3;
				hold(false);
		else:
			player.velocity.y -= friction * delta;
		
