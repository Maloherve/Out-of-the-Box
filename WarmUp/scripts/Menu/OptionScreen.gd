extends Control


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	$Menu/CenterRow/Buttons/Label/Volume.value = AudioServer.get_bus_volume_db(0)


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_ReturnButton_pressed():
	$Click.play()
	yield(get_tree().create_timer(0.1), "timeout")
	get_tree().change_scene("res://scenes/Menu/TitleScreen.tscn")
	


func _on_ReturnButton_mouse_entered():
	$Menu/CenterRow/Buttons/ReturnButton/Label.add_color_override("font_color", Color(0.9,0.1,0.9,0.5))


func _on_ReturnButton_mouse_exited():
	$Menu/CenterRow/Buttons/ReturnButton/Label.add_color_override("font_color", Color(1,1,1,1))





func _on_Volume_value_changed(value):
	AudioServer.set_bus_volume_db(0, value);
	if (value == -24):
		Music.get_node("GameSoundtrack").stop()
	else:
		if (Music.get_node("GameSoundtrack").playing == false):
			Music.get_node("GameSoundtrack").play()
