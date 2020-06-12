extends Area2D

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.



func _on_talk_body_entered(body):
	get_parent().flip_h = true;
	$Label.visible = true;
	yield(get_tree().create_timer(5.0),"timeout")
	$Label.visible = false;
