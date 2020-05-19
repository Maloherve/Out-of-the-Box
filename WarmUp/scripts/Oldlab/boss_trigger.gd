extends "res://scripts/Interactions/zone_trigger.gd"

onready var passage : TileMap = get_node("../../passage");
onready var boss : Node2D = get_tree().get_current_scene().get_node("boss");
#onready var locked_door = get_node("../../locked_door");

func _on_zone_trigger_zone_loading(zone):
	# disable escapes
	passage.set_collision_layer_bit(1, true);
	boss.start();
	disconnect("zone_loading", self, "_on_zone_trigger_zone_loading");

func _init():
	connect("zone_loading", self, "_on_zone_trigger_zone_loading");

