extends Node2D

export (Resource) var packet = load("res://bin/gauss_init1D.gdns").new();
export (Curve) onready var base_potential = null setget set_base_potential; 
export (bool) var debug = false;

const default_mask_bit = 4;

onready var system = get_node("simulator/qsystem");
onready var potential = get_node("simulator/potential");
onready var graph = get_node("simulator/graph");

func set_base_potential(curve):
	if curve != null:
		$base_potential.set_collision_mask_bit(default_mask_bit,true);
	else:
		# disable mask bit
		if has_node("base_potential"):
			$base_potential.set_collision_mask_bit(default_mask_bit,false);
	base_potential = curve;

# Called when the node enters the scene tree for the first time.
func _ready():

	# setup simulator collision mask
	$simulator.set_collision_mask_bit(default_mask_bit,true);
	
	# setup wave
	system.set_wave(packet);
	
	graph.reset();
	potential.reset();
	
	$base_potential.get_node("potential").scale.x = system.width/2;
	$base_potential.set_collision_mask_bit(default_mask_bit, base_potential != null);
	
	if debug:
		print("[wave_function.gd] System position: ", system.mean_position());
		print("[wave_function.gd] System energy: ", system.energy());
		print("[wave_function.gd] System hbar: ", system.hbar);
		print("[wave_function.gd] System mass: ", system.mass);

