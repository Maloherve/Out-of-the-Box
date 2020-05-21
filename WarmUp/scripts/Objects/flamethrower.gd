extends Particles2D

export (float) var switch_time = 4.0;
onready var player : KinematicBody2D = get_tree().get_current_scene().get_node("Player");

func enable():
	self.emitting = true;
	yield(get_tree().create_timer(0.8), "timeout");
	$sound.playing = true;
	$damage_area.enabled = true;

func disable():
	self.emitting = false;
	yield(get_tree().create_timer(0.2), "timeout");
	$sound.playing = false;
	$damage_area.enabled = false;
	
func is_enabled():
	return self.emitting;
	
func _on_timer_timeout():
	if is_enabled():
		disable()
	else:
		enable()	

# Called when the node enters the scene tree for the first time.
func _ready():
	disable();
	$clock.connect("timeout", self, "_on_timer_timeout");
	$clock.start(switch_time);
