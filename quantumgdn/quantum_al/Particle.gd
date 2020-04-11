extends Node2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var simbox = null
var qsystem = null
var n = 21

# Called when the node enters the scene tree for the first time.
func _ready():
	simbox = get_node("simulator")
	qsystem = get_node("simulator/qsystem")
	
	var packet = load("res://bin/gauss_init1D.gdns").new()
	packet.N = 64
	packet.r0 = -25
	packet.k0 = 2 * PI * n / simbox.width 
	packet.sigma = 30
	qsystem.set_wave(packet)
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
