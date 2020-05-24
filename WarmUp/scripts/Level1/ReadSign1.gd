extends Area2D

var target : float = 0.0;
var back_target : float = 0.0;
onready var GameFont = get_tree().get_root().get_node('Level').GameFont;


# Called when the node enters the scene tree for the first time.
func _ready():
	self.connect("body_entered", self, "_on_body_entered");
	self.connect("body_exited", self, "_on_body_exited");
	$Label.percent_visible = 0.0; visible = true;
	boost_quality();
	if has_node("Background"):
		$Background.modulate.a = 0.0;
#	$Label.add_font_override("Game Font", GameFont);


func _on_body_entered(body):
	if body.name == "Player":
		if has_node("Background"):
			back_target = 1;
		target = 2.0;

func _on_body_exited(body):
	if body.name == "Player":
		target = -1;
		if has_node("Background"):
			back_target = -1;


func _process(delta):
	$Label.percent_visible = clamp(lerp($Label.percent_visible, target, .01), 0, 1);
	if has_node("Background"):
		$Background.modulate.a = clamp(lerp($Background.modulate.a, back_target, .01), 0, 0.9);



func boost_quality():
	$Label.get_font("font").size *= 5;
#	$Label.margin_right *= 7;
	$Label.rect_scale = Vector2(0.2, 0.2);
	pass
