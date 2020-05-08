extends Node2D

# Variables for wave function
var x_min : float = -20;				# Minimum value to calculate the wave function
var x_max : float = 20;					# Maximum value to calculate the wave function
var N : int = 1000;						# Precision of the function (number of points)
var step : float= (x_max - x_min)/N;	# Step in x axis (ie distance between two x points)
var x : Array;							# Actual x axis for the Wave Function
var y : Array;							# Value of the wave Function
var change_to_function : bool = false;
var xboost = 10;						# Makes the Wave function longer
# Variables for the gaussian
var amplitude : float = - 10;			# Negative because y positive is down
var fo_x : float = 0;					# x position of the peak
# Player
var can_modify : bool = false;			# Can the wave function be modified by the player
# Other
var draw_wave_function : bool = false;
#var fade_wave_function : bool = false;
# Nodes
onready var player = get_tree().get_root().get_node("Level").get_node("Player")
# Fade mechanism
var fade_timer : Timer = null;
export (float) var fade_time = 1;
export (Curve) var fade_curve = preload("res://assets/Curves/wave_fade.tres"); 

func _ready():
#	Connect Nodes
	player.connect("start_casting", self, "on_Player_start_casting");
	#player.connect("is_casting", self, "on_Player_is_casting")
	player.connect("stop_casting", self, "on_Player_stop_casting");
	#player.connect("finish_casting", self, "on_Player_finish_casting");
#	Make x
	for i in range(N): x.push_back(x_min + i*step);
#	Initialize Wave Function
	for i in range(N):
		y.push_back(amplitude*exp(-x[i]*x[i]/2))
	pass


# Execute ASAP
func _process(delta):
#	Make the wave function appear slowly
	if draw_wave_function:
		if is_fading():
			var t = 1.0 - fade_timer.time_left / fade_time;
			modulate.a = fade_curve.interpolate_baked(t); #lerp(modulate.a, 0, 0.05);
			#print("Remaining: ", fade_timer.time_left, ", Value: ", modulate.a)
		else:
			modulate.a = lerp(modulate.a, 0.5, 0.01);

	
#	Apply changes to function for gaussian case
	if change_to_function:
		for i in range(N):
			y[i] = amplitude*exp(-(x[i] - fo_x)*(x[i] - fo_x)/2);
		change_to_function = false;
	
#	Update draw
	update()


# Execute Regularly
func _physics_process(delta):
	_get_input();
	pass

# Inputs to modify the wave function
func _get_input():
	if Input.is_action_just_pressed("ui_up") && can_modify:
		amplitude -= 1;
		change_to_function = true;
	if Input.is_action_just_pressed("ui_down") && can_modify:
		amplitude += 1;
		change_to_function = true;
	if Input.is_action_just_pressed("ui_left") && can_modify:
		fo_x -= 1;
		change_to_function = true;
	if Input.is_action_just_pressed("ui_right") && can_modify:
		fo_x += 1;
		change_to_function = true;
	pass


func _draw():
	if draw_wave_function:
		draw_function(player.position, y, player.is_on_wall());

func is_fading():
	return fade_timer != null;

# Find a random point in x axis of the Wave Function, weighted by value of Wave Function
func random_point() -> Vector2: 
	var new_pos : Vector2;
#	Find new x and y
	if !player.is_on_wall:
		new_pos.x = player.position.x + xboost * weighted_choice(x, y.duplicate());
		new_pos.y = player.position.y;
	else:
		new_pos.y = player.position.y + xboost * weighted_choice(x, y.duplicate());
		new_pos.x = player.position.x;
	return new_pos;


# ----- Player Action ------
func on_Player_start_casting(trigger):
	set_to_gaussian();
	can_modify = true;
	draw_wave_function = true;

#func on_Player_is_casting():
#	can_modify = true;

func on_Player_stop_casting():
	#fade_wave_function = true;
	can_modify = false;
	# sync the fade animation to a timer
	fade_timer = Timer.new();
	fade_timer.connect("timeout", self, "_on_Timer_timeout");
	fade_timer.process_mode = Timer.TIMER_PROCESS_IDLE;
	fade_timer.one_shot = true;
	fade_timer.set_wait_time(fade_time);
	add_child(fade_timer);
	fade_timer.start();

func _on_Timer_timeout():
#	Make Wave function disapear
	draw_wave_function = false;
	# stop timer
	remove_child(fade_timer);
	fade_timer = null; # will it crash or garbage collecting?
	#fade_wave_function = false;
	modulate.a = 0;
	fo_x = 0;
	amplitude = -10;
	change_to_function = true;
# --------------------------


# ------- Draw Functions ------- 
func draw_path(path, color):
	for i in range(path.get_point_count()-1):
		draw_line(path.get_point_position(i), path.get_point_position(i+1), color, 5)

func draw_function(origin, list, vert):
	var line_width = 3;
	var color = Color(0,1,0);
	var yboost = 7;   # Negative because in Godot, positive goes down
	if !vert:
		var step = (x_max - x_min)/N;
		for i in range(N-1):
			draw_line(origin + Vector2(xboost * x[i], yboost * list[i]), origin + Vector2(xboost * x[i+1], yboost * list[i+1]), color, line_width)
	else:
		if !player.animNode.is_flipped_h():
			yboost = -yboost;
		var step = (x_max - x_min)/N;
		for i in range(N-1):
			draw_line(origin + Vector2(yboost * list[i], xboost * x[i]), origin + Vector2(yboost * list[i+1], xboost * x[i+1]), color, line_width)
# -----------------------------------



# ---- Functions Related to Probability Manipulation -----
# Find a random number within a weighted list
func weighted_choice(numbers, weights) -> float:
	var choice;
#	Normalise
	weights = multiply(weights, 1/sum_of_array(weights));
	weights = cumulative_array(weights);
	var rng = RandomNumberGenerator.new();
	rng.randomize();
	var random = rng.randf();
	for i in range(weights.size()-1):
		if random < weights[i]:
			choice = numbers[i];
			return choice;
	print("There was a problem")
	return choice;


func sum_of_array(array) -> float:
	var sum: float = 0;
	for element in array:
		sum += element;
	return sum;


func cumulative_array(array) -> Array:
	var cum_array : Array
	cum_array.push_back(array[0]);
	for i in range(1,array.size()):
		cum_array.push_back(cum_array[i-1] + array[i]);
	return cum_array;


func multiply(array, constant) -> Array:
	for i in range(array.size()): array[i] *= constant;
	return array


func add(array, constant) -> Array:
	for i in range(array.size()): array[i] += constant;
	return array
# -----------------------------------


# ------- Wave Function -------
func set_to_gaussian():
	for i in range(N):
		y[i] = amplitude*exp(-x[i]*x[i]/2)
	pass
# -----------------------------------


