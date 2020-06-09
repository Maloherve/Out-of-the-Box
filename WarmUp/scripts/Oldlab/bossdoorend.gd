tool
extends "res://scripts/Objects/door.gd"

# Called when the node enters the scene tree for the first time.
func _ready():
	get_parent().connect("starting", self, "_on_boss_starting");
	get_parent().connect("ending", self, "_on_boss_ending");

func _on_boss_starting():
	set_open(true);

func _on_boss_ending():
	set_open(false);
