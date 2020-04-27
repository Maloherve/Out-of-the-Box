extends Area2D

var target : float = 0.0;

# Called when the node enters the scene tree for the first time.
func _ready():
	self.connect("body_entered", self, "_on_body_entered");
	self.connect("body_exited", self, "_on_body_exited");
	$Label.percent_visible = 0; visible = true;

func _on_body_entered(body):
	if body.name == "Player":
		target = 2.0;

func _on_body_exited(body):
	if body.name == "Player":
		target = -1;


func _process(delta):
	$Label.percent_visible = clamp(lerp($Label.percent_visible, target, .01), 0, 1);
