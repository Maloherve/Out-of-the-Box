tool
extends Area2D

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

#export (RectangleShape2D) var rect = RectangleShape2D.new()

#export var width = 10
#export var height = 10
			
func _on_Area2D_input_event( viewport, event, shape_idx ):
	if (event is InputEventMouseButton && event.pressed):
		var T = self.global_transform
		print("Clicked ", T.xform_inv(event.position))

func on_body_entered(node):
	print("WEEE enter")
	
func on_body_exited(node):
	print("WEEE exit")
	
#func _init():
	# ignore ANY scale
	#scale = Vector2(1,1)
	
	
# Called when the node enters the scene tree for the first time.
func _ready():
	input_pickable = true
	connect("input_event", self, "_on_Area2D_input_event")
	connect("body_entered", self, "on_body_entered")
	connect("body_exited", self, "on_body_exited")
	$collider.shape.extents = Vector2(5,5)
	rescale()
	
func rescale():
	$collider.shape.extents *= global_scale
	$collider.global_scale = Vector2(1,1)

func probability(t):
	if t <= 0.0 or t >= 1.0:
		return 0
	else:
		return $qsystem.psi_norm(int($qsystem.N() * t))
	
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
		return $qsystem.x(int((N()-1) * t) + 1)
	
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
