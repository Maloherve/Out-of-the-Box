tool
extends Area2D

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

#export (RectangleShape2D) var rect = RectangleShape2D.new()

export var width = 10
export var height = 10

var system
			
func _on_Area2D_input_event( viewport, event, shape_idx ):
	if (event is InputEventMouseButton && event.pressed):
		var T = self.global_transform
		print("Clicked ", T.xform_inv(event.position))

# Called when the node enters the scene tree for the first time.
func _ready():
	input_pickable = true
	connect("input_event", self, "_on_Area2D_input_event")
	
	var collider = get_node("collider")
	var rect = collider.shape
	rect.extents.x = width / 2.0
	rect.extents.y = height / 2.0
	
	system = get_node("qsystem")
	system.position.x = -width/2
	system.position.y = 0
	system.width = float(width)
	system.height = float(height)
	
	# boundary rectangle check
	var bounds = get_node("boundary")
	bounds.margin_right = width / 2
	bounds.margin_left = -width/2
	bounds.margin_top = -height /2
	bounds.margin_bottom = height/2
	pass # Replace with function body.

func probability(t):
	if t <= 0.0 or t >= 1.0:
		return 0
	else:
		return system.psi_norm(int(system.N() * t))
	
#func probability_from_index(k):
#	return system.psi_norm(k)
	
func N():
	return system.N()
	
func x(t):
	if t <= 0.0:
		return system.x(0)
	elif t >= 1.0:
		return system.width / 2
	else:
		return system.x(int(system.N() * t))
	
func real(t):
	if t <= 0 or t >= 1:
		return 0
	else:
		return system.psi_real(int(system.N() * t))

func imag(t):
	if t <= 0 or t >= 1:
		return 0
	else:
		return system.psi_imag(int(system.N() * t))
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
