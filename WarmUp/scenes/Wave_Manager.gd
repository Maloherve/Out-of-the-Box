extends Node2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"
export (int) var samples = 150
onready var player = get_parent()
var particle = null

# Called when the node enters the scene tree for the first time.
func _ready():
	
	player.connect("start_casting", self, "on_Player_start_casting");
	player.connect("is_casting", self, "on_Player_is_casting")
	player.connect("stop_casting", self, "on_Player_stop_casting");
	player.connect("finish_casting", self, "on_Player_finish_casting");
	
	pass # Replace with function body.

# ----- Player Action ------
func on_Player_start_casting():
	particle = load("res://scenes/Wave/wave_function.tscn")
	var system = particle.get_node('simulator/qsystem')
	var pbox = player.get_node("CollisionShape2D_Cat").shape.extents
	
	# particle Node2D setup
	var half_width
	if player.is_on_wall:
		half_width = pbox.y
		particle.rotation_degree += 90 # node of 90
	else:
		half_width = pbox.x
		
	# boundaries setup, determine them as function of the trigger box
	var simbox = particle.get_node("simulator/collider").shape.extents
	# TODO, pass to the signal
		
	particle.packet = load("res://assets/Other/gauss_init1D.tres")
	# particle setup
	particle.packet.r0 = 0 # center the wave to the player position
	particle.packet.sigma = half_width * 2 # determine sigma as function of the character boundaries
	particle.packet.k0 = 2 * particle.packet.sigma * system.hbar * player.velocity.x *  system.mass / sqrt(PI)
	particle.packet.N = samples
	add_child(particle)

func on_Player_is_casting():
	pass

func on_Player_stop_casting():
	pass

func on_Player_finish_casting():
	remove_child(particle)

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
