tool
extends Area2D

export (String, "intro", "introduction", "walls", "floating_platforms", "sensors", "oldlab") var target_level;
export (int) var checkpoint = -1; 
export (RectangleShape2D) var area = null;

func _on_scene_body_entered(body):
	if body.name == "Player":
		#laisse le temps au son d'etre joué avant de changer de scene 
		$Win.play()
		yield(get_tree().create_timer(0.25), "timeout")
		SceneChanger.change_scene(target_level, checkpoint);

func _init():
	connect("body_entered", self, "_on_scene_body_entered");
	
func _ready():
	assert(area != null)
	$collider.shape = area;
