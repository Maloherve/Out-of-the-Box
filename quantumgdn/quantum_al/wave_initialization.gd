extends Node


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

# Called when the node enters the scene tree for the first time.
func _ready():
	
	# wave function initialization
	var packet = load("res://bin/gaussian_packet.gdns").new()
	packet.first = -50
	packet.second = 50
	packet.x0 = -25
	packet.n = 4
	packet.sigma = 10
	self.Wave = packet.generate()
	
	# potential initialization
	var potential = load("res://bin/uniform_potential.gdns").new()
	potential.value = 5
	self.V = potential
	
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
