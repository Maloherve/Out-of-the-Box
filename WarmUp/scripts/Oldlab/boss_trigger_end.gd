extends Area2D

# Called when the node enters the scene tree for the first time.
func _init():
	connect("body_entered", self, "_on_body_entered", [], CONNECT_ONESHOT);

func _on_body_entered(body):
	if body.get("TYPE") == "PLAYER":
		get_parent().stop();
	
