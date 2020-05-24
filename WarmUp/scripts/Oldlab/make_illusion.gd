extends "res://scripts/Interactions/zone_trigger.gd"

const shader = preload('res://assets/Shaders/big_illusion.shader');
onready var tile : TileMap = get_tree().get_current_scene().get_node("World/Background/illusion");

func _init():
	oneshot = true;
	connect("zone_loading", self, "_on_zone_trigger_zone_loading");
	
func _on_zone_trigger_zone_loading(_zone):
	tile.material.shader = shader;
