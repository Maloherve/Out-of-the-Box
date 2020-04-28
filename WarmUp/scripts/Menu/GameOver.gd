extends Control


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_TryAgainButton_pressed():
	$Click.play()
	get_tree().change_scene("res://scenes/Levels/Introduction.tscn")


func _on_TryAgainButton_mouse_entered():
	$Menu/CenterRow/Buttons/TryAgainButton/Label.add_color_override("font_color", Color(0.9,0.1,0.9,0.5))


func _on_TryAgainButton_mouse_exited():
	$Menu/CenterRow/Buttons/TryAgainButton/Label.add_color_override("font_color", Color(1,1,1,1))
