extends "res://scripts/Interactions/zone_trigger.gd"

onready var passage : TileMap = get_node("../../passage");
#onready var locked_door = get_node("../../locked_door");

func _on_zone_trigger_zone_loading(zone):
	# disable escapes
	passage.set_collision_layer_bit(1, true);

func _init():
	connect("zone_loading", self, "_on_zone_trigger_zone_loading")

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.
