extends Area2D


func _ready():
	self.connect("body_entered", self, "_on_body_entered");




func _on_body_entered(body):
	if body.name == "Player":
		print("tut")
		var flag : bool;
		if name == "Block_TP": flag = false;
		elif name == "Allow_TP": flag = true;
		get_tree().get_root().get_node("Level/Player").allow_casting = flag;
		queue_free()
