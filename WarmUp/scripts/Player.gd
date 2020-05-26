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
# Cast
var allow_casting = true;
var cast : bool = false;
var can_finish_cast = false;
signal start_casting;
signal stop_casting;
# General
var endurance : float = 100;
# Obscuring
const PLAYER_MODULATE_COLOR : Color = Color(0.3,0.3,0.3)
# Audio
var myAudio : AudioStreamPlayer2D;
var myAudioIdle : AudioStreamPlayer2D;

# input state
var locked = false setget set_locked; # disable

# energy 
const DEFAULT_ENERGY = 4.0;
const MIN_ENERGY = 4.0;
const ENERGY_LIMIT = 20.0;
var energy : float = DEFAULT_ENERGY setget set_energy;

# floor VS air
signal landed;
signal falling;
var ground : bool = false;
var air : bool = false;

# dead event
signal dead(reason);

func animate(animation):
	$animator.call(animation);

func set_energy(value):
	if value < MIN_ENERGY:
		value = MIN_ENERGY;
		
func _init():
	connect("landed", self, "_on_landed");
	connect("falling", self, "_on_falling");

func _ready():
	$animator.call("_idle");
	
	_trigger_landing();
	
	# Connect Signals
	$animator.connect("animation_finished", self, "_on_AnimatedSprite_animation_finished");
	$Wave_Generator.connect("teleport", self, "_on_Node_teleport")
	
	connect("start_casting", self, "_on_Player_start_casting");
	
	$death_area.connect("body_entered", self, "_on_death_area_collide");
	
	#$Mover.connect("start_moving", self, "_on_start_moving");
	$Mover.connect("direction_changed", self, "_on_direction_changed");
	$Jumper.connect("jumped", self, "_on_jumped");
	$Climber.connect("hold", self, "_on_wall_hold");
	$Climber.connect("on_ledge", self, "_on_ledge");
		
func _on_direction_changed(move_direction):
	# update look direction
	flip(move_direction > 0);
	
func _on_wall_hold():
	$animator.play("_hold");
	
func _on_jumped():
	$animator.play("_jump");
	
func _on_ledge():
	$animator.play("_walk");
	
func _on_landed():
	$animator.play("_idle");
	
func _on_falling():
	$animator.play("_jump");
	
func set_locked(flag):
	locked = flag;

# Execute ASAP
func _process(delta):
	if !locked:
		_get_input();
			
	_trigger_landing();
	Endurance_Bar.set_value(update_endurance());

# Execute Regularly
func _physics_process(delta):
	if !cast:
		velocity = move_and_slide(velocity, Vector2(0,0));
	
func _trigger_landing():
	if $Bottom_Raycasts.check():
		if !ground:
			emit_signal("landed");
			air = false;
			ground = true;
	elif !check_wall():
		if !air:
			emit_signal("falling");
			air = true;
			ground = false;

func _on_Node_teleport(delta):
	if $Climber.enabled:
		delta -= Vector2($Mover.look * 8,0)
	position += delta
	
func _on_Player_start_casting(_trigger):
	endurance -= 30; # TODO, replace this wth something more logic

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
	
# Check for and execute Input, fast reaction input
func _get_input():
	if Input.is_action_just_pressed("ui_cast"):
			if !cast && endurance>=30 && allow_casting:
				emit_signal('start_casting', null); # no trigger
				cast = true;
				$animator.call("_cast",false);
			elif can_finish_cast:
				emit_signal("stop_casting");
				$animator.call("_endcast")
				cast = false;
				can_finish_cast = false;
	elif Input.is_action_just_released("ui_cast"):
			if cast:
				can_finish_cast = true;
				

func is_front_colliding():
	$Side_Raycasts.is_front_colliding();
	
func front_collisions():
	return $Side_Raycasts.front_collisions();
	
func is_on_ledge():
	return $Side_Raycasts.is_on_ledge();

# See if the character next to a wall
func check_wall():
	return $Side_Raycasts.check();

func update_endurance():
	if $Climber.enabled && endurance>0:
		endurance -= 1;
	if (is_on_floor() && endurance<100 && !cast):
		endurance +=1;
	if endurance<0:
		endurance=0;
	if endurance>100:
		endurance=100;
	return endurance;

# ----- Node Function ------
func _on_AnimatedSprite_animation_finished():
	#print("Finishing animation: ", $animator.get_animation())
	match $animator.get_animation():
		"_walk":
			$animator.call("_idle"); # restore default
			$Trail.emitting = false;
		#"_endcast":
		#	pstate = PSTATE.endcast;
	

# damage
func take_damage(strength):
	energy += strength;
	#velocity.x -= 0.3 * jump_velocity * move_direction;
	#velocity.y += 0.5 * jump_velocity;
	$animator.call("_damage",true);
	if energy > ENERGY_LIMIT:
		die();
	
# death
func die(reason = null):
	print("YOU DIED")
	# TODO, define this behaviour properly
	emit_signal("dead", reason)
	
func _on_death_area_collide(node):
	# simply die
	die(node);
