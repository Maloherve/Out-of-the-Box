extends Sprite

export var energy : float = 2;

# Called when the node enters the scene tree for the first time.
func _ready():
	$Light2D.energy = energy;


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
