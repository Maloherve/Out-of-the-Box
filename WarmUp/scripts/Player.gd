extends KinematicBody2D

# Node describing the movement and actions of the player

# CONSTANTS
const TYPE = "PLAYER";
const GRAVITY : int = 800;
const LIMIT_AIR_SPEED : int = 900;
const MOVE_SPEED : int = 1 * 64;
const CLIMB_SPEED : float = 2.2 * 64;
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
signal move_direction_changed;
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
# group grounded, is_on_wall, is_on_air in a unique field
# reason: they are pairwise disjoint
enum PSTATE { # position state
	ground,
	air,
	wall,
	# temporary states
	jump,
	ledge,
	endcast
};
# PSTATE related signal and member
signal pstate_changed;
var pstate = PSTATE.ground;

# input state
var locked = false setget set_locked; # disable
const input_state = preload('res://scripts/input_state.gd');
var ui_cast = input_state.new("ui_cast");
var ui_up = input_state.new("ui_up");
var ui_down = input_state.new("ui_down");

# energy 
var energy : float = 4.0 setget set_energy;

# dead event
signal dead(reason);

func set_energy(value):
	if value < 2.0:
		value = 2.0;

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
	
	connect("start_casting", self, "_on_Player_start_casting");
	
	connect("pstate_changed", self, "_on_Player_pstate_changed");
	$death_area.connect("body_entered", self, "_on_death_area_collide");
	#connect("move_direction_changed", self, "_on_Player_move_direction_changed")
	
func set_locked(flag):
	self.velocity = Vector2(0,0);
	self.move_direction = 0.0;
	locked = flag;
	
# Execute ASAP
func _process(delta):
	if !attack:
		
		if !locked:
			_get_input();
			
		MoveCharacter(delta);
		
		if (move_direction == -1):
			flip(false);
		elif (move_direction == 1):
			flip(true);
	_trigger_landing();
	
# PSTATE check
func _trigger_landing():
	if !cast :
		# check first the ground	
		if is_bottom_colliding():
			if pstate != PSTATE.ground:
				emit_signal("pstate_changed", PSTATE.ground); # TODO, pass velocity too
				pstate = PSTATE.ground;	
		elif pstate != PSTATE.air:
			if !is_on_wall() || !_check_is_collided() || pstate == PSTATE.endcast:
				emit_signal("pstate_changed", PSTATE.air);
				pstate = PSTATE.air;
				
func _trigger_ledge():
	if is_on_wall() && is_on_ledge() && endurance > 0:
		print("LEDGE")
		pstate = PSTATE.ledge;
		emit_signal("pstate_changed", PSTATE.air);
		pstate = PSTATE.air;

func _trigger_jump():
	if !cast && pstate == PSTATE.ground && endurance > 0:
		pstate = PSTATE.jump;
		emit_signal("pstate_changed", PSTATE.air);
		pstate = PSTATE.air;
		
func _trigger_hold_wall():
	if !cast && !is_on_wall() && _check_is_collided() && endurance > 0:
		emit_signal("pstate_changed", PSTATE.wall)
		pstate = PSTATE.wall
	
# PSTATE changed callback
func _on_Player_pstate_changed(new_state):
	match new_state:
		PSTATE.ground:
			print("[Player] set ground state")
			animNode.call("_idle");
		
		PSTATE.air:
			print("[Player] set air state")
			animNode.call("_jump");
		
			if pstate == PSTATE.jump || pstate == PSTATE.ledge:
				velocity.y = jump_velocity;
		
		PSTATE.wall:
			print("[Player] set wall state")
			animNode.call("_hold");
		
	if pstate == PSTATE.ledge:
		print("[Player] on ledge animation")
		animNode.call("_walk");
		# TODO, use a specific ledge animation
		
func _on_Player_move_direction_changed():
	# update look direction
	flip(move_direction > 0);
	# check for jumps events
	_trigger_jump(); 
		
func is_on_wall():
	return pstate == PSTATE.wall;
	
func look_direction():
	return 2 * int(animNode.flip_h) - 1;

func _on_Node_teleport(delta):
	if pstate == PSTATE.wall:
		delta -= Vector2(look_direction() * 8,0)
	position += delta
	
func _on_Player_start_casting(_trigger):
	endurance -= 30; # TODO, replace this wth something more logic

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
	if pstate == PSTATE.wall && endurance>0:
		velocity.y = lerp(velocity.y, CLIMB_SPEED * vertical_move_direction, 0.2);
	
	#print("Vel y: ", velocity.y)
#	Forces
	if pstate != PSTATE.ground && !cast && velocity.y < LIMIT_AIR_SPEED:
		velocity.y += GRAVITY * delta;
		
	if _check_if_apply_friction():
		#print("[Player] friction applied")
		velocity.y -= 3*GRAVITY/4 * delta;
		
# obscurate, TODO create a child of player to be modulate
func set_obscurate(flag):
	if flag:
		animNode.modulate = lerp(animNode.modulate, PLAYER_MODULATE_COLOR, 0.1)
	else:
		animNode.modulate = lerp(animNode.modulate, Color(1,1,1), 0.01)

func flip(flag):
	animNode.set_flip_h(flag);
	Side_Raycasts.set_flip(!flag);
	$Trail.set_flip(!flag);
	
# Check for and execute Input, fast reaction input
func _get_input():
	match ui_cast.check():
		input_state.ui.just_pressed:
			if !cast && endurance>=30:
				emit_signal('start_casting', null); # no trigger
				cast = true;
				animNode.call("_cast",false);
			elif can_finish_cast:
				emit_signal("stop_casting");
				animNode.call("_endcast")
				cast = false;
				can_finish_cast = false;
		input_state.ui.just_released:
			if cast:
				can_finish_cast = true;
		
	if (ui_up.check() == input_state.ui.just_pressed && !cast):
		_trigger_jump(); # check if possible to jump
		_trigger_ledge();
		
	if (ui_down.check() == input_state.ui.just_pressed && pstate == PSTATE.ground && !cast):
		#attack = true; # temporaly disabled, bugged
		# TODO, emit attack signal
		attackstun = meleeTime;
		
	if Input.is_action_pressed("ui_left") || Input.is_action_pressed("ui_right"):
		_trigger_hold_wall();
		
	if !cast:
		update_move_direction();
	
func update_move_direction():
	# Update move direction
	move_direction = -int(Input.is_action_pressed("ui_left")) + int(Input.is_action_pressed("ui_right"));
		
	if move_direction!=0:
		if move_direction != attack_move_direction:
			attack_move_direction = move_direction;
		if pstate == PSTATE.ground:
			animNode.call("_walk");
			$Trail.emitting = true;
				
	if pstate == PSTATE.wall: 
		vertical_move_direction = -int(Input.is_action_pressed("ui_up")) + int(Input.is_action_pressed("ui_down"));
		if hitstun == 0:
			animNode.call("_hold");

func is_front_colliding():
	return Side_Raycasts.get_node("Top_Side").is_colliding();
	
func front_collisions():
	var out = [];
	for raycast in Side_Raycasts.get_children():
		if raycast.is_colliding():
			out.append(raycast.get_collider());
	return out;
	
func is_bottom_colliding():
	for raycast in Bottom_Raycasts.get_children():
		if raycast.is_colliding():
			return true;
	return false;
	
func bottom_collisions():
	var out = [];
	for raycast in Bottom_Raycasts.get_children():
		if raycast.is_colliding():
			out.append(raycast.get_collider());
	return out;
	
func is_on_ledge():
	return Side_Raycasts.get_node("Bottom_Side").is_colliding() && ! Side_Raycasts.get_node("Top_Side").is_colliding();

# See if the character next to a wall
func _check_is_collided():
	for raycast in Side_Raycasts.get_children():
		if raycast.is_colliding():
			return true;
	return false;

func update_endurance():
	if pstate == PSTATE.wall && endurance>0:
		endurance -= 1;
	if (pstate == PSTATE.ground && endurance<100 && !cast):
		endurance +=1;
	if endurance<0:
		endurance=0;
	if endurance>100:
		endurance=100;
	return endurance;

# Check if the conditions to apply friction are verified
func _check_if_apply_friction():
	return !cast && (pstate == PSTATE.wall && endurance == 0)

# ----- Node Function ------
func _on_AnimatedSprite_animation_finished():
	#print("Finishing animation: ", animNode.get_animation())
	match animNode.get_animation():
		"_walk":
			animNode.call("_idle"); # restore default
			$Trail.emitting = false;
		"_endcast":
			pstate = PSTATE.endcast;
	

# damage
func take_damage(strength):
	energy += strength;
	velocity.x -= 0.3 * jump_velocity * move_direction;
	velocity.y += 0.5 * jump_velocity;
	animNode.call("_damage",true);
	
# death
func die(reason = null):
	print("YOU DIED")
	# TODO, define this behaviour properly
	emit_signal("dead", reason)
	
func _on_death_area_collide(node):
	# simply die
	die(node);
