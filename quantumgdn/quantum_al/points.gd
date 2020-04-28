extends Line2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var qsystem = null
var curve = null

export (float) var gain = 1.0

# Called when the node enters the scene tree for the first time.
func _ready():
	self.scale.y *= -1
	qsystem = get_parent().get_node("qsystem")
	assert(qsystem != null, "Error: null qsystem instance")
	curve = self.material.shader.get_shader_param("points").curve
	pass # Replace with function body.

func point(index):
	return Vector2(qsystem.x(index), qsystem.wave().probability(index) * gain);

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	for i in range(qsystem.N()):
		self.set_point_position(i, point(i))
