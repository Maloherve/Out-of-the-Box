extends Area2D

export var light_path : NodePath;	# Light to make flicker
export var unique : bool = true;	# Only be ably to set the flicker once




# Called when the node enters the scene tree for the first time.
func _ready():
	connect('body_entered', self, '_on_Area2D_body_entered')


func _on_Area2D_body_entered(body):
	get_node(light_path).flicker = true;
	if unique:
		queue_free();
