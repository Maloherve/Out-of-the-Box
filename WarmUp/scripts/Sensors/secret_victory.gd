extends Area2D

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


func _on_secret_victory_body_entered(body):
	if body.get("TYPE") == "PLAYER":
		Jugebox.push_track(null);
		SceneChanger.open_menu("endgame", false, 1.0);
