extends "res://scripts/Level1/ReadSign1.gd"

onready var bar = get_tree().current_scene.get_node("PlayerInterface/EnergyBar");

# Called when the node enters the scene tree for the first time.
func _ready():
	connect("body_entered", self, "hide_bar");
	connect("body_exited", self, "show_bar");
	
func hide_bar(body):
	if body.get("TYPE") == "PLAYER":
		bar.visible = false;
	
func show_bar(body):
	if body.get("TYPE") == "PLAYER":
		bar.visible = true;
