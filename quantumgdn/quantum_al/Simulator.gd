extends Node
# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var qsystem = null
var potential = null

# Called when the node enters the scene tree for the first time.
func _ready():
	
	set_process(false)
		
	# resolve node dependencies
	qsystem = get_node("qsystem")
	potential = get_node("potential")
	
	qsystem.V = potential
	
	# initialize and customize a wave packet
	var packet = load("res://bin/gaussian_packet.gdns").new()
	packet.first = -50
	packet.second = 50
	packet.x0 = -25
	packet.n = 4
	packet.sigma = 10
	packet.size = 100
	
	# initialize the wave with this packet
	qsystem.psi.set_packet(packet)
	pass 

var t = 0

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	t += delta
	print("System energy at ", t, ": ", qsystem.energy())
