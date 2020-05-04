extends Particles2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

#var img = preload("res://star.png")

var qsystem = null

func _init():
	#self.Image = img
	pass

# Called when the node enters the scene tree for the first time.
func _ready():
	# the parent is supposed to be a Simulator
	qsystem = get_parent().get_node("qsystem")
	assert(qsystem != null, "Error: null qsystem instance")
	set_process(true)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	#print("System position: ", self.position.x)
	self.position.x = qsystem.mean_position()
