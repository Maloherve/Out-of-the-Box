extends RigidBody2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	$Area2D.connect("body_entered", self, "on_Area2D_body_entered");
	pass # Replace with function body.


func on_Area2D_body_entered(body):
	print("tut")
