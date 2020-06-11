extends Node2D

# cast the melma
var casting : bool = false;

signal start_casting;
signal end_casting;

const CASTING_TIME : float = 3.84;
const NON_CASTING_TIME : float = 7.68;

const INTRO_WAIT : float = 3.84;

const MELMA_COLLISION_BIT : int = 11;
const DAMAGE : float = 3.0;

signal end_battle;

export (AudioStream) var to_be_played = null;

onready var player = get_tree().get_current_scene().get_node("Player");

# Called when the node enters the scene tree for the first time.
func _init():
	set_process(false);
	
func _ready():
	$melma.modulate.a = 0.0;
	$melma.visible = false;
	$damage_area.enabled = false;
	
func set_enabled_flamethrowers(flag):
	for flame in $flamethrowers.get_children():
		flame.periodic = flag;
	for flame in $constant_flamethrowers.get_children():
		flame.enabled = flag;
	
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
	$damage_area.enabled = true;
	$timer.start(CASTING_TIME);
	$alarm.play();
	yield($melma, "fade_in_completed")
	$firesound.playing = true;
	
func uncast():
	$melma.fade_out(0.5); # fade in and stay
	$damage_area.enabled = false;
	$timer.start(NON_CASTING_TIME);
	$firesound.playing = false;
	
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
		emit_signal("end_casting");
	
func intro():
	player.locked = true; # disable user input
	$intro.start(INTRO_WAIT);
	yield($intro, "timeout");
	
	if to_be_played != null:
		Jugebox.push_track(to_be_played, true);
		
	casting = true;
	cast();
	connect("end_casting", self, "_end_intro_cast");
	
func _end_intro_cast():
	player.locked = false;
	player.set_collision_layer_bit(MELMA_COLLISION_BIT, true);
	set_enabled_flamethrowers(true);
	disconnect("end_casting", self, "_end_intro_cast")
	
signal starting;
signal ending;

func start():
	intro();
	connect_safezones();
	$timer.connect("timeout", self, "_on_Timer_timeout");
	set_process(true);
	emit_signal("starting");
	
func stop():
	$timer.stop();
	$timer.disconnect("timeout", self, "_on_Timer_timeout");
	disconnect_safezones();
	#player.release_damage_layer(MELMA_COLLISION_BIT);
	set_process(false);
	set_process(false);
	set_enabled_flamethrowers(false);
	emit_signal("ending");
