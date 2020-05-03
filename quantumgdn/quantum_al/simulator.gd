tool
extends Area2D

func _ready():
	input_pickable = true
	$collider.shape.extents = Vector2(5,5)
	rescale()
	
func rescale():
	# setup collision box using global scale
	$collider.shape.extents *= global_scale
	$collider.global_scale = Vector2(1,1)

func probability(t):
	if t <= 0.0 or t >= 1.0:
		return 0
	else:
		return $qsystem.psi_norm(int($qsystem.N() * t))
	
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
