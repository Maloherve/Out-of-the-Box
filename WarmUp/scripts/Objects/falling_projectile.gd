extends 'res://scripts/Interactions/projectile.gd'

export (float) var potential_scale = 14 setget set_potential_scale;

var falling : bool = false setget set_falling;

signal self_cancelled(this);

func set_potential_scale(value):
	if get_node("potential") != null:
		$potential.scale.x = value;
	potential_scale = value;

# Called when the node enters the scene tree for the first time.
func _ready():
	set_potential_scale(potential_scale);

func set_falling(value):
	falling = value;
	set_damage_enabled(value);

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if abs(linear_velocity.y) > 0:
		if !falling:
			set_falling(true);
	elif falling:
		set_falling(false);
	
