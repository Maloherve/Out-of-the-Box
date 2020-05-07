extends Area2D

export var light_path : NodePath;	# Light to make flicker
export var unique : bool = true;	# Only be ably to set the flicker once

export var flicker_delay : float = .07;
export var iterations : int = 4;

var LightNode : Sprite;


# Called when the node enters the scene tree for the first time.
func _ready():
	connect('body_entered', self, '_on_Area2D_body_entered')
	LightNode = get_node(light_path)

func _on_Area2D_body_entered(body):
	LightNode.flicker = true;
	LightNode.flicker_delay = flicker_delay;
	LightNode.iterations = iterations;
	if unique:
		queue_free();
