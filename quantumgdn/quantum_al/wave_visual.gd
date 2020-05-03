extends Particles2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

#var img = preload("res://star.png")

onready var qsystem = get_parent().get_node("simulator/qsystem")
onready var scale_x = get_parent().get_node("simulator").scale.x

func _init():
	#self.Image = img
	pass

# Called when the node enters the scene tree for the first time.
func _ready():
	pass
	# the parent is supposed to be a Simulator



# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	#print("System position: ", self.position.x)
	self.position.x = qsystem.mean_position() * scale_x
