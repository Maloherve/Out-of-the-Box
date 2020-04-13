extends KinematicBody2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
	
var pick = false
	
func _input_event(viewport, event, shape_idx):
	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT:
			pick = event.pressed
			
func _input(event):
	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT and not event.pressed:
			pick = false

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta):
	if pick:
		var newpos = get_global_mouse_position()
		var dx = newpos - global_position
		move_and_collide(dx)
	pass
