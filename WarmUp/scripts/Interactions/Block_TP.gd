extends Area2D

onready var caster = get_tree().get_root().get_node("Level/Player/WaveCaster");

func _ready():
	self.connect("body_entered", self, "_on_body_entered");
	caster.turned_on = false;



func _on_body_entered(body):
	if body.name == "Player":
		var flag : bool;
		if name == "Block_TP": flag = false;
		elif name == "Allow_TP": flag = true;
		caster.turned_on = flag;
		queue_free()
