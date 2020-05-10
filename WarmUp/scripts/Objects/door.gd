tool
extends StaticBody2D

# exported variables, only for editor initialization
export (bool) var init_open = false setget set_init_open;
export (float) var init_V = 1.0 setget set_init_V;

# onready variables
onready var open : bool setget set_open, is_open;
onready var potential : float setget set_potential, get_potential;

signal opened;
signal closed;

# init setters
func set_init_open(flag):
	if has_node("animation"): # ready
		set_open(flag);
	init_open = flag;
		
func set_init_V(value):
	if has_node("potential"): # ready
		set_potential(value);
	init_V = value;

# onready setters and getters
func set_open(flag):
	if flag != is_open():
		if flag:
			$animation.play("open");
			set_collision_layer_bit(1,false);
			emit_signal("opened");
		else:
			$animation.play("close");
			set_collision_layer_bit(1,true);
			emit_signal("closed");
	
func is_open():
	return get_collision_layer_bit(1);
	
func set_potential(value):
	$potential.V_x.set_point_value(1, value);

func get_potential():
	return $potential.V_x.get_point_position(1).y

# Called when the node enters the scene tree for the first time.
func _ready():
	set_open(init_open);
	set_potential(init_V);
	$potential.scale.x *= $collider.shape.extents.x;
