extends KinematicBody2D

# Node describing the movement and actions of the player

# CONSTANTS
const TYPE = "PLAYER";
# Nodes
onready var Bottom_Raycasts : Node2D = get_node("Bottom_Raycasts");
onready var Side_Raycasts : Node2D = get_node("Side_Raycasts");
#onready var timer : Timer; # this was before the endurance mechanic, I'm keeping it in case we need it
onready var Endurance_Bar : ProgressBar = get_node("GUI/Endurance_Bar");
# Movement
var velocity : Vector2 = Vector2();

# Attack
var attack_move_direction : int = 1;
var attack : bool = false;
var attackstun : int = 0;
var meleeTime : int = 30;
var hitstun : int = 0; # TODO, when does it change?

# Obscuring
const PLAYER_MODULATE_COLOR : Color = Color(0.3,0.3,0.3)
# Audio
var myAudio : AudioStreamPlayer2D;
var myAudioIdle : AudioStreamPlayer2D;

# input state
var locked = false setget set_locked; # disable

# floor VS air
signal landed;
signal falling;
var ground : bool = false;
var check_landing : bool = true;
var endurance_ground = null;

# cast processes
var endurance_cast = null;
onready var endurance_cast_reactive : int = floor(3 * $Endurance.maximum / 4);

# wall hold processes
var endurance_hold = null;

# energy and death
export (float) var ENERGY_LIMIT = 20.0;

# dead event
signal dead(reason);

func animate(animation):
	$animator.call(animation);
		
func _init():
	connect("landed", self, "_on_landed");
	connect("falling", self, "_on_falling");

func _ready():
	$animator.call("_idle");
	
	if check_landing:
		_trigger_landing();
	
	# Connect Signals
	$animator.connect("animation_finished", self, "_on_AnimatedSprite_animation_finished");
	$WaveCaster.connect("teleport", self, "_on_Node_teleport");
	$WaveCaster.connect("start_casting", self, "_on_start_casting");
	$WaveCaster.connect("stop_casting", self, "_on_stop_casting");
	
	$death_area.connect("body_entered", self, "_on_death_area_collide");
	
	$Mover.connect("moving_step", self, "_on_moving_step");
	$Mover.connect("direction_changed", self, "_on_direction_changed");
	$Jumper.connect("jumped", self, "_on_jumped");
	$Climber.connect("hold", self, "_on_wall_hold");
	$Climber.connect("on_ledge", self, "_on_ledge");
	
	$Endurance.connect("maximum_reached", self, "_on_endurance_max_reached")
	$Endurance.connect("zero_reached", self, "_on_endurance_over");
	$Endurance.connect("value_reached", self, "_on_endurance_value_reached")
	$Endurance.values_to_reach.push_back(endurance_cast_reactive);
	Endurance_Bar.max_value = $Endurance.endurance;
	
func _on_moving_step(running):
	if is_on_floor() && !is_on_wall():
		$animator.call("_walk");
		$Trail.emitting = true;
		if running:
			$Endurance.decrease(4);
		
func _on_direction_changed(move_direction):
	flip(move_direction > 0);
	
func _on_wall_hold(activate):
	if activate:
		$animator.call("_hold");
		endurance_hold = $Endurance.add_decrease_process(3);
		$Jumper.enabled = false;
		reset_ground_endurance();
		ground = false;
	else:
		$Jumper.enabled = true;
		$Endurance.rm_decrease_process(endurance_hold);
	
func _on_jumped():
	reset_ground_endurance();
	$Endurance.decrease(40);
	$jump_sound.play();
	$animator.call("_jump");
	
func _on_ledge():
	$animator.call("_walk");
	
func _on_landed():
	if endurance_ground != null:
		print("ERROR: duplicate endurance_ground")
	else:
		endurance_ground = $Endurance.add_increase_process(3);
		
	if !$Climber.holding:
		$Climber.input_enabled = true;
	$animator.call("_idle");
	
func reset_ground_endurance():
	if endurance_ground != null:
		$Endurance.rm_increase_process(endurance_ground);
		endurance_ground = null;
	
func _on_falling():
	reset_ground_endurance()
	$Jumper.enabled = false;
	$animator.call("_jump");
	
func _on_start_casting(trigger):
	$Mover.enabled = false;
	$Gravity.enabled = false;
	$Jumper.turned_on = false;
	$Climber.hold(false);
	$Climber.enabled = false;
	velocity = Vector2(0,0);
	set_physics_process(false);
	check_landing = false;
	ground = false;
	endurance_cast = $Endurance.add_decrease_process(2);
	reset_ground_endurance()
	$animator.call("_cast");
	
func _on_stop_casting(teleported):
	$Mover.enabled = true;
	$Jumper.turned_on = true;
	$Gravity.enabled = true;
	$Climber.enabled = true;
	set_physics_process(true);
	check_landing = true;
	$Endurance.rm_decrease_process(endurance_cast);
	$animator.call("_endcast");
	
func _on_endurance_max_reached():
	pass
	#$WaveCaster.enabled = true;
	
func _on_endurance_over():
	$Climber.stop_input();
	$Mover.can_run = false;
	$WaveCaster.enabled = false;
	
func _on_endurance_value_reached(value, raising):
	if value == endurance_cast_reactive && raising:
		$WaveCaster.enabled = true;
		$Mover.can_run = true;
	
func set_locked(flag):
	$Mover.set_from_input(!flag);
	$Jumper.turned_on = !flag;
	$WaveCaster.turned_on = !flag;
	locked = flag;

# Execute ASAP
func _process(delta):	
	if check_landing:	
		_trigger_landing();
	Endurance_Bar.set_value($Endurance.endurance);

# Execute Regularly
func _physics_process(delta):
	velocity = move_and_slide(velocity, Vector2(0,-1));
	
func _trigger_landing():
	if is_on_floor():
		if !ground:
			land();
	else:
		if !is_on_wall() && !check_wall():
			emit_signal("falling");
		ground = false;
		
func land():
	emit_signal("landed");
	ground = true;

func _on_Node_teleport(delta):
	if $Climber.holding:
		delta.x -= $Mover.look * 8;
	$teleport_sound.play();
	if $Bottom_Raycasts.check():
		delta.y -= 10;
	position += delta
	
func has_endurance():
	return $Endurance.endurance > 0;
	
# obscurate, TODO create a child of player to be modulate
func set_obscurate(flag):
	if flag:
		$animator.modulate = lerp($animator.modulate, PLAYER_MODULATE_COLOR, 0.1)
	else:
		$animator.modulate = lerp($animator.modulate, Color(1,1,1), 0.01)

func flip(flag):
	$animator.set_flip_h(flag);
	Side_Raycasts.set_flip(!flag);
	$Trail.set_flip(!flag);
	
func look_direction():
	return $Mover.look;
	
func is_holding():
	return $Climber.holding;
	
func vertical_move_direction():
	return $Climber.move;

func is_front_colliding():
	$Side_Raycasts.is_front_colliding();
	
func front_collisions():
	return $Side_Raycasts.front_collisions();
	
func is_on_ledge():
	return $Side_Raycasts.is_on_ledge();
	
func is_running():
	return $Mover.running;

# See if the character next to a wall
func check_wall():
	return $Side_Raycasts.check();

# ----- Node Function ------
func _on_AnimatedSprite_animation_finished():
	#print("Finishing animation: ", $animator.get_animation())
	match $animator.get_animation():
		"_walk":
			$animator.call("_idle"); # restore default
			$Trail.emitting = false;
		#"_endcast":
		#	pstate = PSTATE.endcast;

func set_energy(value):
	$WaveCaster.energy = value;

func change_energy(delta):
	set_energy($WaveCaster.energy + delta);
	
func energy():
	return $WaveCaster.energy;

# damage
func take_damage(strength, stun = null):
	change_energy(strength);
	if stun:
		velocity += stun;
	$animator.call("_damage",true);
	if energy() > ENERGY_LIMIT:
		die();
	
# death

# this is a non elegant solution, 
# don't do it at home coders
var death_timer = null;
var death_reason = null;

func die(reason = null):
	print("YOU DIED")
	$animator.play("_death", true);
	set_locked(false);
	$Mover.enabled = false;
	velocity = Vector2(0,0);
	check_landing = false;
	$Gravity.enabled = false;
	death_reason = reason;
	death_timer = Timer.new();
	death_timer.connect("timeout", self, "_on_real_death");
	add_child(death_timer);
	death_timer.start(2.0);
	# TODO, define this behaviour properly
	
func _on_real_death():
	emit_signal("dead", death_reason)
	remove_child(death_timer);
	
func _on_death_area_collide(node):
	# simply die
	die(node);
