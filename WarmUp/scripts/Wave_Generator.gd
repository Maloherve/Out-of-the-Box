extends Node

# Declare member variables here. Examples:
# var a = 2
# var b = "text"
export (int) var samples = 100
export (Vector2) var scale = Vector2(1,1)
onready var player = get_parent()
var particle = null

# wave function scene preload
const particle_scene = preload("res://scenes/Wave/wave_function.tscn")

# teleport to position
signal teleport

func teleport_position():
	# TODO, Look for the most probable place
	if particle != null:
		var system = particle.get_node('simulator/qsystem')
		return system.mean_position() - system.width/2
	else:
		return 0

func _ready():
	player.connect("start_casting", self, "on_Player_start_casting");
	player.connect("stop_casting", self, "on_Player_stop_casting");
	player.connect("finish_casting", self, "on_Player_finish_casting");

# ----- Player Action ------
# trigger == null: free teleport mode
func on_Player_start_casting(trigger):
	if particle != null:
		return
		
	# load a new simulation
	particle = particle_scene.instance()
	var system = particle.get_node('simulator/qsystem')
	var pbox = player.get_node("CollisionShape2D").shape.extents
	
	# transform setup
	var half_width
	if player.is_on_wall:
		half_width = pbox.y
		particle.rotation_degrees += 90 # node of 90
	else:
		half_width = pbox.x
		
	# boundaries setup, determine them as function of the trigger box
	if trigger != null:
		# TODO, check if working properly
		particle.scale = trigger.box.extents / half_width
	else:
		# scale to the default box (with respect to the cat box)
		particle.scale = scale * half_width / (system.width/2)
		
	# wave setup
	particle.packet = load("res://assets/Other/gauss_init1D.tres")
	
	particle.packet.r0 = system.width/2 # center the wave onto the player position
	particle.packet.sigma = system.width/10 # approximate player quantum localization
	
	# deduce wave vector
	particle.packet.k0 = 2 * particle.packet.sigma * system.hbar * player.velocity.x *  system.mass / sqrt(PI)
	
	# only for debug purposes
	particle.packet.k0 = PI * 50 / system.width
	print("k0 = ", particle.packet.k0)
	
	particle.packet.N = samples
	player.add_child(particle)

func on_Player_is_casting():
	pass

func on_Player_stop_casting():
	# Manage teleporting
	# Use the last computed quantum state to determine the teleport position
	var x = teleport_position()
	var delta = particle.get_relative_transform_to_parent(player).xform_inv(Vector2(x,0))
	delta.y *= -1 # invert y coordinate
	player.position += delta
	
	# deinit simulation
	player.remove_child(particle)
	particle = null

func on_Player_finish_casting():
	pass

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
