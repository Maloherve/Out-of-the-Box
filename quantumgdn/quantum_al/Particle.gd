extends Node2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

export (Resource) var packet = load("res://bin/gauss_init1D.gdns").new()
export (int) var n = 1
onready var system = get_node("simulator/qsystem")

# Called when the node enters the scene tree for the first time.
func _ready():
	
	var potential = get_node("simulator/potential")
	var sim = get_node("simulator")
	print("n = ", n)
	packet.k0 = (2 * PI * n) / sim.width
	print("k0 = ", packet.k0)
	system.set_wave(packet)
	
	get_node("simulator/graph").reset()
	
	for i in range(system.N()):
		potential.add_point(potential.point(i))

	#print("System norm: ", system.norm())
	print("System position: ", system.mean_position())
	print("System energy: ", system.energy())
	print("System hbar: ", system.hbar)
	print("System mass: ", system.mass)

func _process(delta):
	print("System energy: ", system.energy())
