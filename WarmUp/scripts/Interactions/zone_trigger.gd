extends Area2D

export (Resource) var zoneinfo;

func _on_Area2D_BodyExited(_node):
	#var level = get_tree().get_root().get_node("Level");
	var level = get_tree().get_current_scene();
	if zoneinfo != null:
		# load the zone to the level
		level.zone = zoneinfo;

func _init():
	connect("body_exited", self, "_on_Area2D_BodyExited");

