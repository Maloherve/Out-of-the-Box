extends Node

# Declare member variables here. Examples:
# var a = 2
# var b = "text"
export (int) var samples = 100
export (Vector2) var scale = Vector2(1,1)
onready var player = get_parent()
var particle = null

onready var main_scene = get_tree().get_root()

const particle_scene = preload("res://scenes/Wave/wave_function.tscn")

# Called when the node enters the scene tree for the first time.
func _ready():
	
	player.connect("start_casting", self, "on_Player_start_casting");
	player.connect("is_casting", self, "on_Player_is_casting")
	player.connect("stop_casting", self, "on_Player_stop_casting");
	player.connect("finish_casting", self, "on_Player_finish_casting");
	
	pass # Replace with function body.

# ----- Player Action ------
# trigger == null: free teleport mode
func on_Player_start_casting(trigger):
	if particle != null:
		return
		
	# load a new simulation
	particle = particle_scene.instance()
	var system = particle.get_node('simulator/qsystem')
	var pbox = player.get_node("CollisionShape2D").shape.extents
	
	# particle Node2D setup
	var half_width
	if player.is_on_wall:
		half_width = pbox.y
		particle.rotation_degree += 90 # node of 90
	else:
		half_width = pbox.x
		
	# boundaries setup, determine them as function of the trigger box
	
	# adjust system size in order to fit into the box
	if trigger != null:
		# TODO, check if working properly
		particle.scale = trigger.box.extents / half_width
	else:
		# scale to the default box (with respect to the cat box)
		particle.scale = scale * half_width / (system.width/2)
		
	# wave setup
	particle.packet = load("res://assets/Other/gauss_init1D.tres")
	
	particle.packet.r0 = system.width/2 # center the wave to the player position
	particle.packet.sigma = system.width/10
	particle.packet.k0 = 2 * particle.packet.sigma * system.hbar * player.velocity.x *  system.mass / sqrt(PI)
	
	# only for debug purposes
	particle.packet.k0 = PI * 50 / system.width
	
	print("k0 = ", particle.packet.k0)
	
	particle.packet.N = samples
	player.add_child(particle)

func on_Player_is_casting():
	pass

func on_Player_stop_casting():
	player.remove_child(particle)
	particle = null

func on_Player_finish_casting():
	pass

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
