extends Sprite

export var energy : float = 2;
export var color : Color;

export var auto_flicker : bool = false;
export var flicker_delay : float = .07;
export var iterations : int = 4;
export var flicker_in : float = 3.0;

var current_interations : float = 0;
var flicker : bool = false;
var flicker_timer = 0;
var clock = 0;


# Called when the node enters the scene tree for the first time.
func _ready():
	$Light2D.energy = energy;
	$Light2D.color = color;
	modulate = color;

func _physics_process(delta):
	clock += delta;
	if auto_flicker and (clock > flicker_in) and (clock < flicker_in + delta) :
		flicker = true;
		 
	
	if flicker == true:
		flicker_salve(iterations, flicker_delay, delta);

func flicker_salve(iterations, delay,  delta):
	flicker_timer -= 1.0 * delta;
	if (flicker_timer <= 0):
		if ($Light2D.energy > 0):
			$Light2D.energy = 0;
		else:
			$Light2D.energy = energy;
		
		current_interations += .5;
		flicker_timer = flicker_delay;
		if (current_interations >= iterations):
			flicker = false;
			current_interations = 0;
