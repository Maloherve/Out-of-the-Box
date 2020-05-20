extends Area2D

export (String, "intro", "introduction", "walls", "floating_platforms", "sensors", "oldlab") var target_level; 

func _on_scene_body_entered(body):
	if body.name == "Player":
		SceneChanger.change_scene(target_level);

func _init():
	connect("body_entered", self, "_on_scene_body_entered");
	

