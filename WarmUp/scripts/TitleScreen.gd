extends Control


# Called when the node enters the scene tree for the first time.
func _ready():
	pass


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):



#Pressing NewGameButton takes to main scene
func _on_NewGameButton_pressed():
	get_tree().change_scene("res://scenes/Main.tscn")


func _on_NewGameButton_mouse_entered():
	$Menu/CenterRow/Buttons/NewGameButton/Label.add_color_override("font_color", Color(0.9,0.1,0.9,0.5))


func _on_NewGameButton_mouse_exited():
	$Menu/CenterRow/Buttons/NewGameButton/Label.add_color_override("font_color", Color(1,1,1,1))


func _on_OptionsButton_pressed():
	get_tree().change_scene("res://scenes/Menu/OptionSreen.tscn")

