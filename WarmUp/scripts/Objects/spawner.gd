extends Node2D

export (PackedScene) var scene = null;
export (float) var period = 10.0;
var timer : Timer = Timer.new();

# Called when the node enters the scene tree for the first time.
func _init():
	timer.connect("timeout", self, "_on_timer_timeout");
	timer.one_shot = false;
	add_child(timer);
	
func _ready():
	timer.start(period);
	
func _on_timer_timeout():
	var newobj = scene.new();
	add_child(newobj);
