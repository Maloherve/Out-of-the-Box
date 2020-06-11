extends Node2D

export (float) var energy = 4.0 setget set_energy;

func set_energy(value):
	if value > 0.0:
		energy = value;
		
# TODO, color modulation

# Called when the node enters the scene tree for the first time.
func _ready():
	$Area2D.connect("body_entered", self, "_on_Player_entered")

func _on_Player_entered(body):
	if body.get("TYPE") == "PLAYER":
		body.set_energy(energy);
		$AudioStreamPlayer.play();
