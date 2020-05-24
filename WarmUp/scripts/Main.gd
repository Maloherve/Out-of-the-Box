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

# Checkpoints
export (int) var checkpoint = 0; # BUG: setget not working
export (Array, Resource) var checkpoints = Array();

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
	# prevent debugged setups
	if checkpoints.size() == 0:
		var defcheckpt = load("res://scripts/checkpoint.gd").new();
		if has_node("PlayerInitialPosition"):
			defcheckpt.position = $PlayerInitialPosition.position;
		checkpoints.push_back(defcheckpt);
		checkpoint = 0;
		
	set_check_point(checkpoint);
	# Add and activate Camera 
	var camera = Camera2D.new(); camera.name = "Camera2D";
	camera.zoom = camera_zoom
	if activate_camera: camera.current = true;
	$Player.add_child(camera)	
	#SceneChanger.connect("scene_changed", self, "_on_fully_loaded");

func _ready():
	# Connect Signals
	$Player.connect("start_casting", self, "on_Player_start_casting");
	$Player.connect("stop_casting", self, "on_Player_stop_casting");
	$Player.connect("dead", self, "_on_Player_dead");
	
	SceneChanger.detect_current_level();
	
	# setup SceneChanger pointer
	#SceneChanger.level = level_name;

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
func set_zone(zoneinfo):
	assert(zoneinfo != null);
	Jugebox.push_track(zoneinfo.soundtrack);
	self.modulate = zoneinfo.modulate;
		
func set_check_point(number):
	assert(number >= 0 && number < checkpoints.size());
	var checkpt = checkpoints[number];
	set_zone(checkpt.zone);
	if !has_node("Player"):
		yield(self, "tree_entered")
	$Player.position = checkpt.position;
	checkpoint = number;

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
	
# back to menu
func _on_Player_dead(reason):
	# TODO, give hint for the reason
	SceneChanger.open_menu("gameover", false, 2.0);
