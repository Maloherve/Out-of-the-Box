tool
extends Area2D

func _ready():
	#input_pickable = true
	position.x = - $qsystem.width/2
	$collider.position.x = $qsystem.width/2
	$collider.shape.extents = Vector2($qsystem.width/2,$qsystem.width/2)
	rescale()
	connect("body_entered", self, "_on_area_debug")
	
func _on_area_debug(node):
	print("Something entered: ", node)
	
func rescale():
	# setup collision box using global scale
	$collider.shape.extents *= global_scale
	$collider.global_scale = Vector2(1,1)

func probability(t):
	if t <= 0.0 or t >= 1.0:
		return 0
	else:
		return $qsystem.psi_norm(int($qsystem.N() * t))
		
func V(t):
	if t <= 0.0:
		return $qsystem.potential_at_index(0) # first potential point
	elif t >= 1.0:
		return $qsystem.potential_at_index($qsystem.N() - 1)
	else:
		return $qsystem.potential_at_index(int($qsystem.N() * t))
	
func N():
	return $qsystem.N()
	
func x(t):
	if t <= 0.0:
		return $qsystem.x(0)
	elif t >= 1.0:
		return $qsystem.width
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
