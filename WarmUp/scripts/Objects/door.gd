tool
extends StaticBody2D

# exported variables, only for editor initialization
export (bool) var init_open = false setget set_init_open;
#export (float) var init_V = 1.0 setget set_init_V;

# onready variables
onready var open : bool setget set_open;

export (Array) var layer_bits = [1, 5];

signal opened;
signal closed;

# init setters
func set_init_open(flag):
	if has_node("animation"): # ready
		set_open(flag);
	init_open = flag;
	
func set_bits(flag):
	for b in layer_bits:
		set_collision_layer_bit(b,flag);
		
# onready setters and getters
func set_open(flag):
	if flag:
		$animation.play("open");
		set_bits(false);
		emit_signal("opened");
	else:
		$animation.play("close");
		set_bits(true);
		emit_signal("closed");
	open = flag;


func _ready():
	set_open(init_open);
