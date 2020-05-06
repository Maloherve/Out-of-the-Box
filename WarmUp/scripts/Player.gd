extends KinematicBody2D

# Node describing the movement and actions of the player

# CONSTANTS
const TYPE = "PLAYER";
const GRAVITY : int = 800;
const MOVE_SPEED : int = 1 * 64;
const CLIMB_SPEED : float = 1.5 * 64;
# Nodes
onready var animNode : AnimatedSprite;
onready var Bottom_Raycasts : Node2D = get_node("Bottom_Raycasts");
onready var Side_Raycasts : Node2D = get_node("Side_Raycasts");
#onready var timer : Timer; # this was before the endurance mechanic, I'm keeping it in case we need it
onready var Endurance_Bar : ProgressBar = get_node("GUI/Endurance_Bar");
# Movement
var velocity : Vector2 = Vector2();
var jump_velocity : int = -388;
var Up : Vector2 = Vector2(0, -1);
var move_direction : int = 1;
var vertical_move_direction : int = 1;
var is_on_wall : bool = false;
var is_on_ledge : bool = false;
#var climbing_timeout : bool = false; # for the climbing time limit
# Attack
var attack_move_direction : int = 1;
var attack : bool = false;
var attackstun : int = 0;
var meleeTime : int = 30;
var hitstun : int = 0; # TODO, when does it change?
# Cast
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
# Ground
signal hit_the_ground;
signal detach_the_ground;
var grounded : bool = true;

func _ready():
	animNode = get_node("AnimatedSprite")
	if (animNode != null):
		animNode.call("_idle");
	
	# Connect Signals
	animNode.connect("animation_finished", self, "_on_AnimatedSprite_animation_finished");
	$Wave_Generator.connect("teleport", self, "_on_Node_teleport")
	
	#timer = Timer.new();
	#add_child(timer);
	#timer.connect("timeout", self, "_on_timer_timeout");
	
	connect("start_casting", self, "_on_Player_start_casting")
	
	connect("hit_the_ground", self, "_on_hit_the_ground")
	connect("detach_the_ground", self, "_on_detach_the_ground")

# Execute ASAP
func _process(delta):
	if !attack:
		_get_input();
		MoveCharacter(delta);
		if (move_direction == -1):
			animNode.set_flip_h(false);
			Side_Raycasts.set_flip(true);
			$Trail.set_flip(true);
		elif (move_direction == 1):
			animNode.set_flip_h(true);
			Side_Raycasts.set_flip(false);
			$Trail.set_flip(false);
	$Trail.emitting = (animNode.get_animation() == "_walk")
	_check_is_landed();
	
func look_direction():
	return 2 * int(animNode.flip_h) - 1;

func _on_Node_teleport(delta):
	if is_on_wall:
		delta -= Vector2(look_direction() * 8,0)
	position += delta
	
func _on_Player_start_casting(_trigger):
	endurance = 0;

# Execute Regularly
func _physics_process(delta):
	if !cast:
		velocity = move_and_slide(velocity, Vector2(0,0));
	Endurance_Bar.set_value(update_endurance());

# Move Character
func MoveCharacter(delta):
#	Determine the new Velocities
#	x-Movement
	if (!attack && !cast):
		velocity.x = lerp(velocity.x, MOVE_SPEED * move_direction, 0.2);
	
#	y-Movement
	if is_on_wall && endurance>0:
		velocity.y = lerp(velocity.y, CLIMB_SPEED * vertical_move_direction, 0.2);
		
#	_climbing_timer();
	
#	Forces
	if _check_if_apply_gravity():
		velocity.y += GRAVITY * delta;
		
	if _check_if_apply_friction():
		velocity.y -= GRAVITY/2 * delta;
		
# obscurate, TODO create a child of player to be modulate
func set_obscurate(flag):
	if flag:
		animNode.modulate = lerp(animNode.modulate, PLAYER_MODULATE_COLOR, 0.1)
	else:
		animNode.modulate = lerp(animNode.modulate, Color(1,1,1), 0.01)

func _on_hit_the_ground():
	is_on_wall = false;
	animNode.call("_idle");
	# TODO reset all other possible variables
	# TODO animate the impact
	
func _on_detach_the_ground():
	animNode.call("_jump");

func _input(event):
	if event.is_action_pressed("ui_space") && !cast:
		emit_signal('start_casting', null); # no trigger
		cast = true;
		animNode.call("_cast",true)
		#if !timer.is_stopped():
		#	timer.set_paused(true);
	if event.is_action_released("ui_space") && cast:
		can_finish_cast = true
	
# Check for and execute Input
func _get_input():
	if (Input.is_action_just_pressed("ui_up") && !cast):
		if grounded || is_on_ledge:
			velocity.y = jump_velocity;
	if (Input.is_action_just_pressed("ui_down") && grounded && !cast):
		#attack = true; # temporaly disabled, bugged
		# TODO, emit attack signal
		attackstun = meleeTime;
	if (Input.is_action_just_pressed("ui_space") && (!cast) && (endurance>=50)):
		emit_signal('start_casting', null); # no trigger
		cast = true;
		endurance -= 50;
#		if !timer.is_stopped():
#			timer.set_paused(true);
	# cast immediate reaction
	if Input.is_action_pressed("ui_space") && can_finish_cast:
		emit_signal("stop_casting");
		animNode.call("_endcast")
		cast = false;
		can_finish_cast = false;
		#if timer.is_paused():
		#	timer.set_paused(false);
	
	# Update move direction
	if (!cast):
		move_direction = -int(Input.is_action_pressed("ui_left")) + int(Input.is_action_pressed("ui_right"));
		
		if move_direction!=0:
			if move_direction != attack_move_direction:
				attack_move_direction = move_direction;
			if grounded:
				animNode.call("_walk")
				
		if (Input.is_action_just_pressed("ui_up") && (grounded || is_on_ledge)):
				velocity.y = jump_velocity; 
				
		if (Input.is_action_just_pressed("ui_down")):
			#attack = true;
			attackstun = meleeTime;
			#animNode.call("_attack",true)
			
		if is_on_wall: 
			vertical_move_direction = -int(Input.is_action_pressed("ui_up")) + int(Input.is_action_pressed("ui_down"));
			if hitstun == 0:
				animNode.call("_hold")


# Use Raycast to see if the character is on the ground
func _check_is_landed():
	for raycast in Bottom_Raycasts.get_children():
		if raycast.is_colliding():
			#timer.stop();
			#climbing_timeout = false;
			if !grounded:
				grounded = true;
				emit_signal("hit_the_ground"); # TODO, pass velocity too
			return;
	if grounded:
		grounded = false;
		emit_signal("detach_the_ground");

func is_front_colliding():
	return Side_Raycasts.get_node("Top_Side").is_colliding();

# See if the character next to a wall
func _check_is_collided():
	if Side_Raycasts.get_node("Bottom_Side").is_colliding() && ! Side_Raycasts.get_node("Top_Side").is_colliding():
		is_on_ledge = true;
	else:
		is_on_ledge = false;
	for raycast in Side_Raycasts.get_children():
		if raycast.is_colliding():
			return true;
	return false;

# Time limit on climbing
#func _on_timer_timeout():
#	climbing_timeout = true;
#	return;

#func _climbing_timer():
#	if is_on_wall && timer.is_stopped() && !climbing_timeout:
#		timer.start(1);
#	return;

func update_endurance():
	if is_on_wall && endurance>0:
		endurance -= 1;
	if (grounded && endurance<100 && (!cast)):
		endurance +=1;
	return endurance;

# Check if the conditions to apply gravity are verified
func _check_if_apply_gravity():
	if cast:
		return false;
	if !grounded && _check_is_collided() && endurance > 0:
		is_on_wall = true;
		return false;
	return true;

# Check if the conditions to apply friction are verified
func _check_if_apply_friction():
	return !cast && (is_on_wall && endurance == 0)

# ----- Node Function ------
func _on_AnimatedSprite_animation_finished():
	#print("Finishing animation: ", animNode.get_animation())
	if (animNode.get_animation() == "_walk"):
		animNode.call("_idle"); # restore default
	#if (animNode.get_animation() == "_attack"):
	#	attackstun = meleeTime;

