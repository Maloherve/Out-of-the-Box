extends Area2D

func _ready():
	self.connect("body_entered", self, "_on_body_entered");


func _on_body_entered(body):
	if body.name == "Player":
		get_tree().get_root().get_node("Level/Player").die("Area not available");
