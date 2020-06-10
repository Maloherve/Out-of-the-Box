extends Area2D

onready var caster = get_tree().get_root().get_node("Level/Player/WaveCaster");
onready var energybar = get_tree().get_current_scene().get_node("PlayerInterface/EnergyBar");

func _ready():
	self.connect("body_entered", self, "_on_body_entered");
	caster.turned_on = false;
	energybar.visible = false;



func _on_body_entered(body):
	if body.name == "Player":
		var flag : bool;
		if name == "Block_TP": flag = false;
		elif name == "Allow_TP": flag = true;
		caster.turned_on = flag;
		energybar.visible = flag;
		queue_free()
