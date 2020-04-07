extends Node2D

# This node describes the effects in the world, for exemple the darkening when casting

# For Debug
var debug_mode : bool = false;
# Darken Workd
var darken : bool = false;
const WORLD_MODULATE_COLOR : Color = Color(0.5,1,0.5);
const PLAYER_MODULATE_COLOR : Color = Color(0.3,0.3,0.3)
# Player 
var teleport_player : bool = false;

func _ready():
	# Connect Signals
	$Player.connect("start_casting", self, "on_Player_start_casting");
	$Player.connect("stop_casting", self, "on_Player_stop_casting");


# Execute ASAP
func _process(delta):
#	Darken Screen When Casting
	if (darken):
		$World.modulate = lerp($World.modulate, WORLD_MODULATE_COLOR, 0.01);
		$Player.modulate = lerp($Player.modulate, PLAYER_MODULATE_COLOR, 0.1);
	else:
		$World.modulate = lerp($World.modulate, Color(1,1,1), 0.05)
		$Player.modulate = lerp($Player.modulate, Color(1,1,1), 0.01)
	
#	Toggle Debug Screen
	if debug_mode:
		$InfoScreen.show();
	else:
		$InfoScreen.hide();


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
	var new_pos : Vector2 = $Fonction_Onde.rand_pos()
	
	
	
	
	
	pass

# Finds the new position in the wave
func find_position_wave():
	pass


# ----- Node Function ------
func on_Player_start_casting():
	darken = true;

func on_Player_stop_casting():
	teleport_player = true;
	darken = false;
