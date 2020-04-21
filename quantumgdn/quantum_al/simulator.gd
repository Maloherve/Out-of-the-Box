tool
extends Area2D

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

#export (RectangleShape2D) var rect = RectangleShape2D.new()

export var width = 10
export var height = 10
			
func _on_Area2D_input_event( viewport, event, shape_idx ):
	if (event is InputEventMouseButton && event.pressed):
		var T = self.global_transform
		print("Clicked ", T.xform_inv(event.position))

# Called when the node enters the scene tree for the first time.
func _ready():
	print("simulator _ready")
	
	input_pickable = true
	connect("input_event", self, "_on_Area2D_input_event")
	
	var collider = get_node("collider")
	var rect = collider.shape
	rect.extents.x = width / 2.0
	rect.extents.y = height / 2.0
	
	var system = get_node("qsystem")
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
	
	print("Width = ", width)
	print("Height = ", height)
	
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
