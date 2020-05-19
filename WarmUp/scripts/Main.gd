extends Node2D

# This node describes the effects in the world, for exemple the darkening when casting



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
export var level_number : int;


# zone wrapper
export (Resource) var zone setget set_zone;

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

	
	

func _ready():
	# Connect Signals
	$Player.connect("start_casting", self, "on_Player_start_casting");
	$Player.connect("stop_casting", self, "on_Player_stop_casting");
	
	set_zone(zone, true); # export + setget


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

# Setup a new zone inside the scene
func set_zone(zoneinfo, force = false):
	if zoneinfo != null && (force || zone != zoneinfo):
		if has_node("mainsound"):
			$mainsound.push_track(zoneinfo.soundtrack);
		if zoneinfo.modulate != null:
			self.modulate = zoneinfo.modulate;
		zone = zoneinfo;

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
