extends Control

export var play_music : bool = true;
var volume = AudioServer.get_bus_volume_db(0)
var soundtrack = Music.get_node("GameSoundtrack")
onready var video_player = get_node("VideoPlayer")
onready var video = preload("res://assets/Background/tv_effect.ogv")

# Called when the node enters the scene tree for the first time.
func _ready():
	if play_music:
		if (soundtrack.playing == false && volume > -24):
			soundtrack.play()
	
	SceneChanger.get_node("Control").visible = false
	video_player.set_stream( video )
	set_process( true )



# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if not video_player.is_playing():
		video_player.play()


#Pressing NewGameButton takes to main scene
func _on_NewGameButton_pressed():
	SceneChanger.get_node("Control").visible = true
	$Click.play()
	SceneChanger.change_scene("res://scenes/Levels/Intro.tscn", 0.5)
	


func _on_NewGameButton_mouse_entered():
	$Menu/CenterRow/Buttons/NewGameButton/Label.add_color_override("font_color", Color(0.9,0.1,0.9,0.5))


func _on_NewGameButton_mouse_exited():
	$Menu/CenterRow/Buttons/NewGameButton/Label.add_color_override("font_color", Color(1,1,1,1))


func _on_OptionsButton_pressed():
	$Click.play()
	#laisse le temps au son d'etre joué avant de changer de scene
	yield(get_tree().create_timer(0.1), "timeout") 
	get_tree().change_scene("res://scenes/Menu/OptionScreen.tscn")



func _on_OptionsButton_mouse_entered():
	$Menu/CenterRow/Buttons/OptionsButton/Label.add_color_override("font_color", Color(0.9,0.1,0.9,0.5))
	


func _on_OptionsButton_mouse_exited():
	$Menu/CenterRow/Buttons/OptionsButton/Label.add_color_override("font_color", Color(1,1,1,1))
