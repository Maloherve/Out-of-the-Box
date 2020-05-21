extends Resource

const zoneinfo = preload("res://scripts/zoneinfo.gd");

export (String) var name;
export (Resource) var zone;
export (Vector2) var position;

func _init(var name = "Checkpoint", var zone = zoneinfo.new(), var position = Vector2()):
	self.name = name;
	self.zone = zone;
	self.position = position;
