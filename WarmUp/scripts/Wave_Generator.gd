extends Node

export (int) var samples = 100
export (Vector2) var scale = Vector2(1,1)
export (Vector2) var offset = Vector2(0,0)

onready var player : KinematicBody2D = get_parent()
var particle : Node2D = null
var detectors : Array = []

const SCALE_FACTOR = 0.02;

# cast variables
export (bool) var enabled = true setget set_enabled;
export (String) var action = "ui_cast";
var cast : bool = false;
var can_finish_cast = false;
signal start_casting;
signal stop_casting;

# wave function scene preload
const particle_scene = preload("res://scenes/Wave/wave_function.tscn")

# teleport to position
signal teleport

func set_enabled(flag):
	set_process(flag);
	enabled = flag;
	
func _process(_delta):
	if Input.is_action_just_pressed(action):
		if !cast:
			emit_signal('start_casting', null); # no trigger
			cast = true;
			$animator.call("_cast",false);
		elif can_finish_cast:
			emit_signal("stop_casting");
			$animator.call("_endcast")
			cast = false;
			can_finish_cast = false;
	elif Input.is_action_just_released(action):
			if cast:
				can_finish_cast = true;
	

func winning_detector_position(system):
	var P = 0
	var winner = -1
	var index = 0
	for detector in detectors:
		var prob = detector.detect(system)
		print("Detector: ", index, ", P = ", prob)
		if prob > P:
			P = prob
			winner = index
		index += 1
	if winner >= 0: # it should always happen
		var T = system.get_global_transform().affine_inverse() * detectors[winner].get_global_transform();
		return T.xform(Vector2(0,0)).x;
	else:
		print("WARNING: no winner detector found")
		return system.width / 2

func teleport_position():
	# TODO, Look for the most probable place
	if particle != null:
		var system = particle.get_node('simulator/qsystem')
		# there must be at least a couple of detectors
		if detectors.size() < 2:
			return system.mean_position()
		else:
			print("Evaluating position with detectors")
			return winning_detector_position(system);
	else:
		return 0

# comparison criteria for the detection
func minor_index(lhs, rhs):
	return lhs.detector_index < rhs.detector_index
	
# get the detector index in the array
# if the node is not present it returns -1	
func get_detector(node):
	var found = detectors.bsearch_custom(node, self, "minor_index", true)
	if !detectors.empty() && detectors[found] == node:
		return found
	else:
		return -1
		
func _on_Area2D_Area_entered(node):
	#print("Node Area2D found: ", node, ", valid :", node.has_node("detectable"))
	if particle != null && node is Area2D && node.has_node("detectable"):
		# check if not already present int the detectors array
		if get_detector(node) < 0:
			if detectors.empty():
				node.detector_index = 0
			else:
				node.detector_index = detectors.back().detector_index + 1
			#print("Detector found")
			detectors.push_back(node)
		
func _on_Area2D_Area_exited(node):
	var index = get_detector(node)
	if index >= 0:
		node.detector_index = -1
		detectors.remove(index)

func _ready():
	player.connect("start_casting", self, "on_Player_start_casting");
	player.connect("stop_casting", self, "on_Player_stop_casting");
	
# ----- Player Action ------
# trigger == null: free teleport mode
func on_Player_start_casting(trigger):
	if particle != null:
		return
		
	# load a new simulation
	particle = particle_scene.instance()
	var system = particle.get_node('simulator/qsystem')
	particle.get_node("simulator").connect("area_entered", self, "_on_Area2D_Area_entered")
	particle.get_node("simulator").connect("area_exited", self, "_on_Area2D_Area_exited")
	var pbox = player.get_node("CollisionShape2D").shape.extents
	
	# transform setup
	var direction
	var half_width
	var dx = 0.0 # small displacement
	if player.is_on_wall():
		half_width = pbox.y
		particle.rotation_degrees -= 90 * player.look_direction() # node of 90
		# fix no move
		direction = -player.vertical_move_direction * player.look_direction();
	else:
		half_width = pbox.x
		direction = player.look_direction()
		if player.is_front_colliding():
			dx = -5
		
	# boundaries setup, determine them as function of the trigger box
	if trigger != null:
		# TODO, check if working properly
		particle.scale = trigger.box.extents / half_width
	else:
		var gain = exp(SCALE_FACTOR * (player.energy - player.DEFAULT_ENERGY));
		if gain > 1.4:
			gain = 1.4; # limit in size reached
		# scale to the default box (with respect to the cat box)
		particle.scale.x = scale.x * half_width / (system.width/2)* gain;
		particle.scale.y = scale.y * half_width / (system.width/2);
		
	particle.position = offset
		
	# wave setup
	particle.packet = load("res://assets/Other/gauss_init1D.tres")
	
	particle.packet.r0 = system.width/2 + dx * direction # center the wave onto the player position
	
	# correct r0 as function of the raycasts
	
	particle.packet.sigma = system.width/20 # approximate player quantum localization
	
	# deduce wave vector
	#particle.packet.k0 = 2 * particle.packet.sigma * system.hbar * player.velocity.x *  system.mass / sqrt(PI)
	
	# only for debug purposes
	particle.packet.k0 = direction * PI * 50 / system.width
	print("[Wave_Generator.gd] k0 = ", particle.packet.k0)
	
	# modify hbar as function of the energy
	system.mass = player.DEFAULT_ENERGY / player.energy;
	
	particle.packet.N = samples
	player.add_child(particle)
	

func on_Player_stop_casting():
	# Manage teleporting
	# Use the last computed quantum state to determine the teleport position
	var x = teleport_position()
	var delta = particle.get_node("simulator/qsystem").get_relative_transform_to_parent(player).xform(Vector2(x,0))
	emit_signal("teleport", delta)
	
	# deinit simulation
	player.remove_child(particle)
	particle = null


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
