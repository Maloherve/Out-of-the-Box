extends "res://scripts/Interactions/zone_trigger.gd"

onready var walls : TileMap = get_tree().current_scene.get_node("World/walls");
onready var corridor : TileMap = get_tree().current_scene.get_node("World/corridor_upper");

func _init():
	connect("zone_loading", self, "_on_zone_trigger_zone_loading");
	
func _on_zone_trigger_zone_loading(_zone):
	walls.modulate = Color(1.0, 1.0, 1.0, 1.0);
	corridor.modulate = Color(1.0, 1.0, 1.0, 1.0);
