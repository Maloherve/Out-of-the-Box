tool
extends Particles2D

# enabled
export (bool) var enabled = true setget set_enabled;

# periodic
export (bool) var periodic = true setget set_periodic;
var clock : Timer = Timer.new();

export (float) var switch_time = 4.0;
export (float) var shift = 0.0;

#onready var player : KinematicBody2D = null;

# ease in / fade out mechanism
export (float) var ease_time = 1.0;
var easing_in : bool = false;
var fading_out : bool = false;

# collision shape
var collshape = CapsuleShape2D.new();

# constants
const RADIUS = 9.963;

const MAX_HEIGHT = 142.924;
const MAX_POSITION = 92.213;
const INIT_FIN_HEIGHT = 21.113;
const INIT_POSITION = 28.213;
const FINAL_POSITION = 155.213;

# yield
var delay_state : GDScriptFunctionState = null;

var ready = false;

func _init():
	collshape.radius = RADIUS;
	collshape.height = MAX_HEIGHT;
	
func set_enabled(value):
	if !easing_in && !fading_out:
		if ready:
			self.emitting = value;
			$sound.playing = value;
			$damage_area.enabled = value;
			if ease_time > 0:
				if value:
					ease_in();
				else:
					fade_out();
		enabled = value;
		
func set_periodic(value):
	if ready:
		if value:
			if shift != 0:
				delay_state = yield(get_tree().create_timer(shift), "timeout");
			if !clock.is_connected("timeout", self, "_on_timer_timeout"):
				clock.connect("timeout", self, "_on_timer_timeout");
			if clock.is_stopped():
				clock.start(switch_time);
		else:
			if delay_state != null:
				delay_state.resume();
			if !clock.is_stopped():
				clock.stop();
			if clock.is_connected("timeout", self, "_on_timer_timeout"):
				clock.disconnect("timeout", self, "_on_timer_timeout");
	periodic = value;
	
func ease_in():
	$fader.interpolate_property(collshape, "height", INIT_FIN_HEIGHT, MAX_HEIGHT, ease_time, Tween.TRANS_LINEAR, Tween.EASE_IN);
	$fader.interpolate_property($damage_area/collider, "position:x", INIT_POSITION, MAX_POSITION, ease_time, Tween.TRANS_LINEAR, Tween.EASE_IN);
	$fader.start();
	easing_in = true;
	
func fade_out():
	$fader.interpolate_property(collshape, "height", MAX_HEIGHT, INIT_FIN_HEIGHT, ease_time, Tween.TRANS_LINEAR, Tween.EASE_IN);
	$fader.interpolate_property($damage_area/collider, "position:x", MAX_POSITION, FINAL_POSITION, ease_time, Tween.TRANS_LINEAR, Tween.EASE_IN);
	$fader.start();
	fading_out = true;
	
func _on_timer_timeout():
	set_enabled(!enabled);	
		
func _on_Tween_completed():
	easing_in = false;
	fading_out = false;
	if periodic:
		clock.start(switch_time);

# Called when the node enters the scene tree for the first time.
func _ready():
	ready = true;
	clock.one_shot = true;
	add_child(clock);
	
	$damage_area/collider.position.x = MAX_POSITION;
	$damage_area/collider.shape = collshape;
	$fader.connect("tween_all_completed", self, "_on_Tween_completed");
	
	set_enabled(enabled);
	set_periodic(periodic);
	
	#if get_tree().current_scene != null:
	#	player = get_tree().get_current_scene().get_node("Player");
	
