extends TileMap

var onthefloor : bool = false setget onthefloor_set, onthefloor_get;
var disable_timer : Timer = null;

onready var player : KinematicBody2D = get_tree().get_current_scene().get_node("Player");

func onthefloor_set(flag):
	set_collision_layer_bit(1,flag);
	
func onthefloor_get():
	return get_collision_layer_bit(1);
	
func disable():
	onthefloor_set(false);
	set_collision_layer_bit(10,false);
	disable_timer = Timer.new();
	disable_timer.connect("timeout", self, "reenable");
	add_child(disable_timer);
	disable_timer.start(0.5);
	
func reenable():
	set_collision_layer_bit(10,true);
	remove_child(disable_timer);
	disable_timer = null;

# Called when the node enters the scene tree for the first time.
func _ready():
	player.connect("pstate_changed", self, "_on_Player_pstate_changed");
	
func is_bottom_colliding():
	var coll = player.bottom_collisions();
	for obj in coll:
		if self == obj:
			return true;
	return false;
	
func _input(event):
	if event.is_action_pressed("ui_down") && onthefloor_get():
		disable();
	
func _on_Player_pstate_changed(newstate):
	if newstate == player.PSTATE.ground && is_bottom_colliding():
		onthefloor_set(true);
	elif onthefloor_get():
		onthefloor_set(false);
