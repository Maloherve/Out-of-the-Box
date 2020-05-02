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

func _ready():
	#input_pickable = true
	#connect("input_event", self, "_on_Area2D_input_event")
	pass
	
func resize():
	var rect = $collider.shape
	rect.extents.x = width / 2.0
	rect.extents.y = height / 2.0
	
	$qsystem.position.x = -width/2
	$qsystem.position.y = 0
	$qsystem.width = float(width)
	$qsystem.height = float(height)
	
	# boundary rectangle check
	$boundary.margin_right = width / 2
	$boundary.margin_left = -width/2
	$boundary.margin_top = -height /2
	$boundary.margin_bottom = height/2

func probability(t):
	if t <= 0.0 or t >= 1.0:
		return 0
	else:
		return $qsystem.psi_norm(int(N() * t))
	
#func probability_from_index(k):
#	return system.psi_norm(k)
	
func N():
	return $qsystem.N()
	
func x(t):
	if t <= 0.0:
		return $qsystem.x(0)
	elif t >= 1.0:
		return $qsystem.width / 2
	else:
		return $qsystem.x(int(N() * t))
	
func real(t):
	if t <= 0 or t >= 1:
		return 0
	else:
		return $qsystem.psi_real(int(N() * t))

func imag(t):
	if t <= 0 or t >= 1:
		return 0
	else:
		return $qsystem.psi_imag(int(N() * t))
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
