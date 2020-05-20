extends Node2D

# cast the melma
var casting : bool = false;

signal start_casting;

const CASTING_TIME : float = 3.0;
const NON_CASTING_TIME : float = 6.0;

const INTRO_WAIT : float = 8.0;

const MELMA_COLLISION_BIT : int = 11;

export (AudioStream) var to_be_played;

onready var player = get_tree().get_current_scene().get_node("Player");

# Called when the node enters the scene tree for the first time.
func _init():
	set_process(false);
	
func _ready():
	$melma.modulate.a = 0.0;
	$melma.visible = false;
	
func connect_safezones():
	for safezone in $safezones.get_children():
		safezone.connect("body_entered", self, "_on_Player_safezone_enter");
		safezone.connect("body_exited", self, "_on_Player_safezone_exit");
	
func disconnect_safezones():
	for safezone in $safezones.get_children():
		safezone.disconnect("body_entered", self, "_on_Player_safezone_enter");
		safezone.disconnect("body_exited", self, "_on_Player_safezone_exit");
	
func cast():
	$melma.fade_in(0.5); # fade in and stay
	$damage_area.set_collision_mask_bit(MELMA_COLLISION_BIT, true);
	$timer.start(CASTING_TIME);
	#print("Cast")
	
func uncast():
	$melma.fade_out(0.5); # fade in and stay
	$damage_area.set_collision_mask_bit(MELMA_COLLISION_BIT, false);
	$timer.start(NON_CASTING_TIME);
	#print("Uncast")
	
func _on_Player_safezone_enter(body):
	if body == player:
		player.set_collision_layer_bit(MELMA_COLLISION_BIT, false);
	
func _on_Player_safezone_exit(body):
	if body == player:
		player.set_collision_layer_bit(MELMA_COLLISION_BIT, true);
	
func _on_Timer_timeout():
	casting = !casting;
	if casting: # eval new state
		cast();
		emit_signal("start_casting");
	else:
		uncast();
	
	
func _on_Intro_timeout():
	# really start the bossfight
	player.locked = false;
	player.set_collision_layer_bit(MELMA_COLLISION_BIT, true);
	
	if to_be_played != null:
		Jugebox.push_track(to_be_played);
	uncast();
	
func intro():
	$intro.connect("timeout", self, "_on_Intro_timeout");
	$intro.start(INTRO_WAIT);
	player.locked = true;  # disable user input
	$melma.fade_in_out(5.0, 2.0); # enter and exit for 5 sec, with a delay of 1 sec
	

func start():
	intro();
	connect_safezones();
	$timer.connect("timeout", self, "_on_Timer_timeout");
	set_process(true);
	
func stop():
	$timer.disconnect("timeout", self, "_on_Timer_timeout");
	disconnect_safezones();
	player.release_damage_layer(MELMA_COLLISION_BIT);
	set_process(false);
