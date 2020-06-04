tool
extends StaticBody2D

# exported variables, only for editor initialization
export (RectangleShape2D) var collider = RectangleShape2D.new() setget set_collider; 
export (SpriteFrames) var frames = null setget set_frames;
export (float) var V = 4.0 setget set_V;

export (Vector2) var sprite_scale = Vector2(1, 1) setget set_sprite_scale;
export (Vector2) var sprite_position = Vector2(0, 0) setget set_sprite_position;

# onready variables
export (bool) var open setget set_open;

export (Array) var layer_bits = [1, 5];

var is_ready = false;

signal opened;
signal closed;

# potential
func set_V(pot):
	if is_ready:
		$potential.V_x.set_point_value(1, pot);
	V = pot;
	
# collision
func set_collider(shape):
	if is_ready:
		$CollisionShape2D.shape = shape;
	collider = shape;
	
# sprite
func set_frames(var entry):
	if is_ready:
		$animation.frames = entry;
	frames = entry;

func set_sprite_scale(sc):
	if is_ready:
		$animation.scale = sc;
	sprite_scale = sc;
	
func set_sprite_position(pos):
	if is_ready:
		$animation.position = pos;
	sprite_position = pos;
	
func set_bits(flag):
	for b in layer_bits:
		set_collision_layer_bit(b,flag);
		
# onready setters and getters
func set_open(flag):
	if is_ready:
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
	is_ready = true;
	set_open(open);
	set_V(V);
	set_frames(frames);
	set_sprite_position(sprite_position);
	set_sprite_scale(sprite_scale);
	set_collider(collider);
