extends "res://scripts/Level1/ReadSign1.gd"

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var activated : bool = false;


# Called when the node enters the scene tree for the first time.
func _init():
	set_process_input(true);

func _on_RealVictory_body_entered(body):
	if body.get("TYPE") == "PLAYER":
		activated = true;
	
func _on_RealVictory_body_exited(body):
	if body.get("TYPE") == "PLAYER":
		activated = false;
	
func _input(event):
	if activated && event.is_action_pressed("ui_space"):
		Jugebox.push_track(null);
		SceneChanger.open_menu("endgame", false, 1.0);

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
