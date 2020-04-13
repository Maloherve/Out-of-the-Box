extends Node2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var simbox = null
var qsystem = null
var graph = null
var wave = null
export (Resource) var packet = load("res://bin/gauss_init1D.gdns").new()
export (int) var n = 1

# Called when the node enters the scene tree for the first time.
func _ready():
	simbox = get_node("simulator")
	qsystem = get_node("simulator/qsystem")
	graph = get_node("simulator/graph")
	wave = qsystem.wave()

	#qsystem.set_physics_process(false) # comment on to evolve
	
	simbox.width = 600
	simbox.height = 250
	packet.k0 = 2 * PI * n / simbox.width
	qsystem.set_wave(packet)
	
	for i in range(qsystem.N()):
		graph.add_point(graph.point(i))
		#print("point", i, "=", graph.get_point_position(i))
	
	print("System N: ", qsystem.N())
	print("System position: ", qsystem.position())
	print("System energy: ", qsystem.energy())
	print("System hbar: ", qsystem.hbar)
	print("System mass: ", qsystem.mass)
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	#print("System position: ", qsystem.position())
	pass
