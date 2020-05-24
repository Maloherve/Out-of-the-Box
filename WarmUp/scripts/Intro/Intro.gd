extends Node2D

# This node describes the effects in the world, for exemple the darkening when casting
var dialog = [
	'So...', 
	'How is it going with Subject 1926?', 
	'Any progress?',
	'None so far... We had high hopes for Schrödy- um, I mean Subject 1926.',
	'As you know, he is our latest attempt at creating state superposition at a macroscopic scale',
	'Originally it looked like the artificial enlargement of his de Broglie wavelength had succeeded',
	'However, so far we haven\'t observed any of the expected quantum phenomena...',
	'It seems like the superposition state can\'t be triggered by an external source...',
	'Maybe Schrödy needs to will himself into a probability wave?',
	'Don\'t be ridiculous. Cats don\'t have a consciousness.',
	'In any case, we don\'t have time to test every outlandish hypothesis you come up with.',
	'We need some results, and fast.',
	'Proceed with the Schrödinger experiment.',
	'What? But we haven\'t even asserted his ability to enter superposition!',
	'In his current state, there\'s no way of knowing if he would survi-',
	'It doesn\'t matter, we have plenty of other test subjects ready for you.',
	'Just do your job.',
	'The president himself is counting on you.'
	]
var anim = ['redTalkStart', 'redTalk', 'redTalkEnd', 'blueTalkStart', 'blueTalk', 'blueTalk', 'blueTalk', 'blueTalk', 'blueTalkEnd', 'talkRed', 'talkRed', 'talkRed', 'talkRed', 'talkBlue', 'talkBlue', 'talkRed', 'talkRed', 'leaving' ]
var page = 0

const steps = [
  'Footsteps_Casual_Concrete_09',
  'Footsteps_Casual_Concrete_10',
  'Footsteps_Casual_Concrete_11',
  'Footsteps_Casual_Concrete_12'
]

onready var dialogBox = get_node('World/NPC/DialogueBox/Dialogue')
# Game font
var GameFont : Font = load('assets/Fonts/abel-regular.ttf');
# For Debug
var debug_mode : bool = false;
# Darken Workd
var darken : bool = false;
const WORLD_MODULATE_COLOR : Color = Color(0.5,1,0.5);
#const PLAYER_MODULATE_COLOR : Color = Color(0.3,0.3,0.3)
# Player 
var camera_zoom : Vector2 = Vector2(.3, .3)
export var activate_camera : bool = true;
export var cat_version : int = 1;
var teleport_player : bool = false;
# TMP


func _enter_tree():
	# Load in Player
	var player_scene;
	match cat_version:
		0:
			player_scene = load("res://scenes/Human.tscn")
		1:
			player_scene = load("res://scenes/Cat.tscn")
		2:
			player_scene = load("res://scenes/Cat2.tscn")
	
	var Player = player_scene.instance()
	add_child(Player)
	Player.position = $PlayerInitialPosition.position
	# Add and activate Camera 
	var camera = Camera2D.new(); camera.name = "Camera2D";
	camera.zoom = camera_zoom
	if activate_camera: camera.current = true;
	$Player.add_child(camera)
	
	Player.visible = false
	Player.set_locked(true);
	
	
func _ready():
	# Connect Signals
	$Player.connect("start_casting", self, "on_Player_start_casting");
	$Player.connect("stop_casting", self, "on_Player_stop_casting");
	$World/NPC/Box.play('eyes')
	$World/NPC/DialogueBox.visible = false


# Execute ASAP
func _process(delta):
#	Darken Screen When Casting
	if (darken):
		$World.modulate = lerp($World.modulate, WORLD_MODULATE_COLOR, 0.01);
		$Player.set_obscurate(true);
		#$Player.modulate = lerp($Player.modulate, PLAYER_MODULATE_COLOR, 0.1);
	else:
		$World.modulate = lerp($World.modulate, Color(1,1,1), 0.05)
		$Player.set_obscurate(false);
		#$Player.modulate = lerp($Player.modulate, Color(1,1,1), 0.01)
	
#	Toggle Debug Screen
	if debug_mode:
		$InfoScreen.show();
	else:
		$InfoScreen.hide();
	
	# Temporaly hidden, See Player.Wave_Generator.on_Player_stop_casting()
	#if (teleport_player):
	#	teleport();
	

# Execute Regularly
func _physics_process(delta):
	_get_input();


# Check for and execute input
func _get_input():
	if Input.is_action_just_pressed("ui_tab"):
		debug_mode = !debug_mode;


# Teleports the character
func teleport():
	# Wave returns a random posistion
	var new_pos : Vector2 = $Fonction_Onde.random_point()
	$Player.position = new_pos;
	$Fonction_Onde.set_to_gaussian();
	teleport_player = false;


# ----- Node Function ------
func on_Player_start_casting(trigger):
	darken = true;
	AudioServer.set_bus_effect_enabled ( 1, 0, true )

func on_Player_stop_casting():
	$Teleport.play();
	teleport_player = true;
	darken = false;
	AudioServer.set_bus_effect_enabled ( 1, 0, false )
	
func _input(event):
	if $World/NPC/DialogueBox.visible && event is InputEventMouseButton && event.is_pressed():
		if event.button_index == BUTTON_LEFT:
			if dialogBox.get_visible_characters() > dialogBox.get_total_character_count():
				if page < dialog.size()-1:
					page += 1
					dialogBox.set_bbcode(dialog[page])
					dialogBox.set_visible_characters(0)
					$World/Enemies/Scientists.play(anim[page])
					if anim[page] == "blueTalkStart" or \
					anim[page] == "blueTalk" or anim[page] == "blueTalkEnd" \
					or anim[page] == "talkBlue":
						print(anim[page])
						$World/NPC/DialogueBox/Speaker.add_color_override("font_color", Color(0.1,0.6,0.9,1))
					else:
						$World/NPC/DialogueBox/Speaker.add_color_override("font_color", Color(0.8,0.1,0.1,1))
			else:
				dialogBox.set_visible_characters(dialogBox.get_total_character_count())



func _on_EventTimer_timeout():
	$World/NPC/Box.play('jump')
	$World/EventTimer.wait_time = 0.5
	if $World/NPC/DialogueBox/Arrow.visible:
		$World/NPC/DialogueBox/Arrow.visible = false
	else:
		$World/NPC/DialogueBox/Arrow.visible = true


func _on_Box_animation_finished():
	if ($World/NPC/Box.animation == 'jump'):
		$World/NPC/DialogueBox.visible = true
		dialogBox.set_bbcode(dialog[page])
		dialogBox.set_visible_characters(0)
		dialogBox.set_process_input(true)
		$World/Enemies/Scientists.play('redTalkStart')


func _on_Box_frame_changed():
	if ($World/NPC/Box.animation == 'jump'):
		if ($World/NPC/Box.frame == 3):
			$BoxJump.play()

func _on_Timer_timeout():
	dialogBox.set_visible_characters(dialogBox.get_visible_characters()+1)
	


func _on_Scientists_animation_finished():
	if ($World/Enemies/Scientists.animation == 'blueTalkStart'):
		$World/Objects/machine.play('laser')
	if ($World/Enemies/Scientists.animation == 'leaving'):
		$World/NPC/DialogueBox.visible = false
		SceneChanger.change_scene("introduction", -1, 0.5)
		
func play_random_step():
	var rand_nb = randi() % steps.size()
	var audiostream = load("res://assets/Sounds/Footsteps/" + steps[rand_nb] + '.wav')
	$World/Enemies/footsteps.stream = audiostream
	$World/Enemies/footsteps.play()


func _on_Scientists_frame_changed():
	if ($World/Enemies/Scientists.animation == 'leaving'):
		if ($World/Enemies/Scientists.frame > 7 && $World/Enemies/Scientists.frame % 2):
			play_random_step()
