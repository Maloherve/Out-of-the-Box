extends Node2D

export (Resource) var packet = load("res://bin/gauss_init1D.gdns").new()
export (bool) var debug = false

onready var system = get_node("simulator/qsystem")
onready var sim = $simulator
onready var potential = get_node("simulator/potential")
onready var graph = get_node("simulator/graph")

# Called when the node enters the scene tree for the first time.
func _ready():

	# update simulator size, duplicate call
	#$simulator.rescale()
	
	system.set_wave(packet)
	
	graph.reset()
	potential.reset()
	
	if debug:
		print("[wave_function.gd] System position: ", system.mean_position())
		print("[wave_function.gd] System energy: ", system.energy())
		print("[wave_function.gd] System hbar: ", system.hbar)
		print("[wave_function.gd] System mass: ", system.mass)

