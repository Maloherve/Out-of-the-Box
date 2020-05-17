extends Area2D

export (Resource) var zoneinfo;

func _on_Area2D_BodyExited(_node):
	var level = get_tree().root;
	if zoneinfo != null:
		# load the zone to the level
		level.zone = zoneinfo;

func _init():
	connect("body_exited", self, "_on_Area2D_BodyExited");


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
