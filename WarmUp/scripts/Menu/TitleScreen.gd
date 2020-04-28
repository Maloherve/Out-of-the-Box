extends Control


# Called when the node enters the scene tree for the first time.
func _ready():
	pass


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):



#Pressing NewGameButton takes to main scene
func _on_NewGameButton_pressed():
	$Click.play()
	get_tree().change_scene("res://scenes/Levels/Introduction.tscn")
	


func _on_NewGameButton_mouse_entered():
	$Menu/CenterRow/Buttons/NewGameButton/Label.add_color_override("font_color", Color(0.9,0.1,0.9,0.5))


func _on_NewGameButton_mouse_exited():
	$Menu/CenterRow/Buttons/NewGameButton/Label.add_color_override("font_color", Color(1,1,1,1))


func _on_OptionsButton_pressed():
	$Click.play()
	#laisse le temps au son d'etre joué avant de changer de scene
	yield(get_tree().create_timer(0.1), "timeout") 
	get_tree().change_scene("res://scenes/Menu/OptionSreen.tscn")



func _on_OptionsButton_mouse_entered():
	$Menu/CenterRow/Buttons/OptionsButton/Label.add_color_override("font_color", Color(0.9,0.1,0.9,0.5))
	


func _on_OptionsButton_mouse_exited():
	$Menu/CenterRow/Buttons/OptionsButton/Label.add_color_override("font_color", Color(1,1,1,1))
