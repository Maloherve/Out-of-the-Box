extends Node2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

export (Resource) var packet = load("res://bin/gauss_init1D.gdns").new()
#export (int) var n = 1
export (bool) var debug = false

#onready var player = get_tree().get_root().get_node("Level").get_node("Player")
var system = get_node("simulator").get_node("qsystem")
var potential = get_node("simulator").get_node("potential")

# Called when the node enters the scene tree for the first time.
func _ready():

	# update simulator size
	$simulator.resize()
	
	#print("n = ", n)
	#packet.k0 = (2 * PI * n) / $simulator.width
	#print("k0 = ", packet.k0)
	system.set_wave(packet)
	
	get_node("simulator/graph").reset()
	
	for i in range(system.N()):
		potential.add_point(potential.point(i))
	
	if debug:
		print("System position: ", system.mean_position())
		print("System energy: ", system.energy())
		print("System hbar: ", system.hbar)
		print("System mass: ", system.mass)

