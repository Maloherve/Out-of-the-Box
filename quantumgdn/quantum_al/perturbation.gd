extends KinematicBody2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

export var domain = 50.0

# Called when the node enters the scene tree for the first time.
func _ready():
	var field = get_node("field")
	var influence = get_node("influence")
	field.scale.x = domain 
	print("Points: ", field.V_x.get_point_count())
	for k in range(-10,10,1):
		var x = 2 * k * domain / 20
		var Vx = field.at(Vector2(x,0))
		influence.add_point(Vector2(x, -Vx)) 
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
