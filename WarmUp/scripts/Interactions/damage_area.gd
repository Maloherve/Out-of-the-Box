tool
extends Area2D

export (int) var DAMAGE_MASK_BIT = 11;

export (float) var damage : float = 3.0;

var enabled : bool = true setget set_enabled;

onready var player = get_tree().get_current_scene().get_node("Player");

func _init():
	connect("body_entered", self, "_on_damage_area_enter");

func set_enabled(value):
	set_collision_mask_bit(DAMAGE_MASK_BIT, value);
	enabled = value;

func _on_damage_area_enter(body):
	if body == player:
		player.take_damage(damage);
	
