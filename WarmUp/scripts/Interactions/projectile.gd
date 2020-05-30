extends RigidBody2D

const DAMAGE_BIT : int = 11;
export (bool) var damage_enabled = true setget set_damage_enabled, get_damage_enabled;
export (float) var damage = 0.0;
onready var player = get_tree().get_current_scene().get_node("Level/Player");

# Called when the node enters the scene tree for the first time.
func _init():
	connect("body_entered", self, "_on_body_entered");
	
func set_damage_enabled(flag):
	set_collision_mask_bit(DAMAGE_BIT, flag);
	
func get_damage_enabled():
	return get_collision_mask_bit(DAMAGE_BIT);

func _on_body_entered(body):
	if body == player && damage_enabled:
		player.take_damage(damage);

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
