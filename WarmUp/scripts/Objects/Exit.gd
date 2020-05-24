extends Node2D

onready var ExitLight_Sprite : Sprite = get_node("ExitSign/ExitLight");
onready var ExitLight_Light : Light2D = get_node("ExitSign/Light2D");

var timer : float = 0.0;
export var long_wait : float = 2.0;
var on_time : float = 1;
var current_flag : bool = true;

func _ready():
	set_on(false);



func _process(delta):
	timer += delta;
	if belongs(timer,0,long_wait): set_on(false);
	elif belongs(timer,long_wait,long_wait+on_time): set_on(true);
	else: timer = 0;


func set_on(flag):
	if flag and flag != current_flag:
		ExitLight_Sprite.modulate = Color(0, 200, 0, 255) / 255;
		ExitLight_Light.enabled = true;
	elif flag != current_flag:
		ExitLight_Sprite.modulate = Color(30, 125, 45, 255) / 255;
		ExitLight_Light.enabled = false;
	current_flag = flag;

func belongs(x, xmin, xmax):
	if (x > xmin) and (x < xmax):
		return true;
	else:
		return false;
