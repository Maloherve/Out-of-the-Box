extends Node

export (int) var speed = 140;
export (int) var friction = 400; 
export (bool) var enabled = false setget set_enabled;

onready var player = get_parent();

var move : int = 0;

signal hold;
signal on_ledge;

func set_enabled(flag):
	set_physics_process(flag);
	if flag:
		emit_signal("hold");	
	enabled = flag;

func update_move_from_input():
	move = - int(Input.is_action_pressed("ui_up")) + int(Input.is_action_pressed("ui_down")); 

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta):
	if move != 0:
		player.velocity.y = lerp(player.velocity.y, speed * move, 0.2);
		if player.is_on_ledge() && Input.is_action_just_pressed("ui_up"):
			emit_signal("on_ledge");
			enabled = false;
	else:
		player.velocity.y -= friction * delta;
		
