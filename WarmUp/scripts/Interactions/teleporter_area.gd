extends Area2D
# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


func _on_teleporter_body_entered(body):
	if body.get("TYPE") == "PLAYER":
		body.position = $destination.global_position;
