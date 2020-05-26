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

# energy 
const DEFAULT_ENERGY = 4.0;
const MIN_ENERGY = 4.0;
const ENERGY_LIMIT = 20.0;
var energy : float = DEFAULT_ENERGY setget set_energy;

# floor VS air
signal landed;
signal falling;
var ground : bool = false;
var check_landing : bool = true;

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
	
func _on_moving_step():
	if is_on_floor() && !is_on_wall():
		$animator.play("_walk");
		$Trail.emitting = true;
		
func _on_direction_changed(move_direction):
	flip(move_direction > 0);
	
func _on_wall_hold(activate):
	if activate:
		$animator.play("_hold");
		$Endurance.automatic_increase = false;
		$Endurance.automatic_decrease = true;
		$Jumper.enabled = false;
		ground = false;
	else:
		$Jumper.enabled = true;
		$Endurance.automatic_decrease = false;
	
func _on_jumped():
	$jump_sound.play();
	$Endurance.automatic_increase = false;
	$animator.play("_jump");
	
func _on_ledge():
	$animator.play("_walk");
	
func _on_landed():
	$Endurance.automatic_increase = true;
	$Endurance.automatic_decrease = false;
	$Climber.input_enabled = true;
	$animator.play("_idle");
	
func _on_falling():
	$animator.play("_jump");
	
func _on_start_casting(trigger):
	$Mover.enabled = false;
	$Gravity.enabled = false;
	$Jumper.enabled = false;
	$Climber.hold(false);
	$Climber.enabled = false;
	velocity = Vector2(0,0);
	_physics_process(false);
	check_landing = false;
	$Endurance.automatic_decrease = true;
	$animator.call("_cast");
	
func _on_stop_casting(teleported):
	$Mover.enabled = true;
	$Jumper.enabled = true;
	$Gravity.enabled = true;
	$Climber.enabled = true;
	_physics_process(true);
	check_landing = true;
	$Endurance.automatic_decrease = false;
	$animator.call("_endcast");
	
func _on_endurance_max_reached():
	$WaveCaster.enabled = true;
	
func _on_endurance_over():
	$Climber.stop_input();
	$WaveCaster.enabled = false;
	
func set_locked(flag):
	$Mover.set_from_input(!flag);
	$Jumper.enabled = !flag;
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
			emit_signal("landed");
			ground = true;
	else:
		if !is_on_wall() && !check_wall():
			emit_signal("falling");
		ground = false;

func _on_Node_teleport(delta):
	if $Climber.holding:
		delta -= Vector2($Mover.look * 8,0)
	$teleport_sound.play();
	position += delta
	
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
