extends "res://scripts/Interactions/zone_trigger.gd"

onready var passage : TileMap = get_node("../../passage");
onready var walls : TileMap = get_node("../../walls");
onready var boss : Node2D = get_tree().get_current_scene().get_node("boss");
#onready var locked_door = get_node("../../locked_door");

func _on_zone_trigger_zone_loading(zone):
	# disable escapes
	passage.set_collision_layer_bit(1, true);
	passage.modulate *= Color(1.0, 0.2, 0.2, 1.0);
	walls.modulate *= Color(1.0, 0.2, 0.2, 1.0);
	walls.z_index += 2;
	boss.start();
	disconnect("zone_loading", self, "_on_zone_trigger_zone_loading");
	yield(boss, "end_battle");
	walls.modulate /= Color(1.0, 0.2, 0.2, 1.0);
	walls.z_index -= 2;

func _init():
	connect("zone_loading", self, "_on_zone_trigger_zone_loading");

