extends Line2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var qsystem = null
var area = null

# Called when the node enters the scene tree for the first time.
func _ready():
	qsystem = get_parent().get_node("qsystem")
	area = get_parent()
	assert(qsystem != null, "Error: null qsystem instance")
	pass # Replace with function body.

func point(index):
	return Vector2(qsystem.x(index), qsystem.wave().real(index) * area.height);

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	for i in range(qsystem.N()):
		self.set_point_position(i, point(i))
