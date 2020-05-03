extends Line2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

onready var qsystem = get_parent().get_node("qsystem")

export (float) var gain = 1

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

func point(index):
	return Vector2(qsystem.x(index), qsystem.potential_at_index(index) * gain);

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	for i in range(qsystem.N()):
		self.set_point_position(i, point(i))

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
