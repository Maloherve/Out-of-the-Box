extends Area2D

export (bool) var enabled = true;
export (Resource) var zoneinfo;
signal zone_loading;

export (bool) var oneshot = false;

func _on_Area2D_BodyEnter(_node):
	#var level = get_tree().get_root().get_node("Level");
	var level = get_tree().get_current_scene();
	if zoneinfo != null:
		# load the zone to the level
		level.set_zone(zoneinfo);
		emit_signal("zone_loading", zoneinfo);
		if oneshot:
			disconnect("body_entered", self, "_on_Area2D_BodyEnter");

func _init():
	if enabled:
		connect("body_entered", self, "_on_Area2D_BodyEnter");

