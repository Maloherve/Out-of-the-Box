extends Node2D

# Variables for wave function
var x_min : float = -20;				# Minimum value to calculate the wave function
var x_max : float = 20;					# Maximum value to calculate the wave function
var N : int = 1000;						# Precision of the function (number of points)
var step : float= (x_max - x_min)/N;	# Step in x axis (ie distance between two x points)
var y : Array;							# Value of the wave Function
var change_to_function : bool = false;
# Variables for the gaussian
var amplitude : float = 10;				# Amplitude of the Wave Function
var fo_x : float = 0;					# x position of the peak
# Player
var can_modify : bool = false;			# Can the wave function be modified by the player
# Other
var draw_wave_function : bool = false;
var fade_wave_function : bool = false;
# Nodes
onready var player = get_tree().get_root().get_node("Level").get_node("Player")


func _ready():
#	Connect Nodes
	player.connect("start_casting", self, "on_Player_start_casting");
	player.connect("is_casting", self, "on_Player_is_casting")
	player.connect("stop_casting", self, "on_Player_stop_casting");
	player.connect("finish_casting", self, "on_Player_finish_casting");
#	Determine Wave Function
	set_to_gaussian();


# Execute ASAP
func _process(delta):
#	Make the wave function appear slowly
	if draw_wave_function:
		if fade_wave_function:
			modulate.a = lerp(modulate.a, 0, 0.05);
		else:
			modulate.a = lerp(modulate.a, 0.5, 0.01);
	
#	Apply changes to function
	if change_to_function:
		for i in range(N+1):
			var x = x_min + i*step + fo_x;
			y[i] = amplitude*exp(-x*x/2);
		change_to_function = false;
	
#	Update draw
	update()


# Execute Regularly
func _physics_process(delta):
	_get_input();
	pass


func _get_input():
	if Input.is_action_just_pressed("ui_up") && can_modify:
		amplitude += 1;
		change_to_function = true;
	if Input.is_action_just_pressed("ui_down") && can_modify:
		amplitude -= 1;
		change_to_function = true;
	if Input.is_action_just_pressed("ui_left") && can_modify:
		fo_x += 1;
		change_to_function = true;
	if Input.is_action_just_pressed("ui_right") && can_modify:
		fo_x -= 1;
		change_to_function = true;
	pass


func _draw():
	if draw_wave_function:
		draw_function(player.position, y, false);
	pass


# Return a random position 
func rand_pos():
	var new_pos : Vector2;
	
	
	
	return new_pos;





# ----- Player Action ------
func on_Player_start_casting():
	set_to_gaussian();
	draw_wave_function = true;

func on_Player_is_casting():
	can_modify = true;

func on_Player_stop_casting():
	fade_wave_function = true;
	can_modify = false;

func on_Player_finish_casting():
#	Make Wave function disapear
	draw_wave_function = false;
	fade_wave_function = false;
	modulate.a = 0;
	fo_x = 0;
	amplitude = 10;
	change_to_function = true;
# --------------------------

# ------- Draw Functions ------- 
func draw_path(path, color):
	for i in range(path.get_point_count()-1):
		draw_line(path.get_point_position(i), path.get_point_position(i+1), color, 5)

func draw_function(origin, list, vert):
	var line_width = 3;
	var color = Color(0,1,0);
	if !vert:
		var yboost = -7;   # Negative because in Godot, positive goes down
		var xboost = 10;
		var step = (x_max - x_min)/N;
		for i in range(N):
			var x = x_min + i*step;
			var x_ = x_min + (i+1)*step;
			draw_line(origin + Vector2(xboost * x, yboost * list[i]), origin + Vector2(xboost * x_, yboost * list[i+1]), color, line_width)
# -----------------------------------


# ------- Wave Function -------
func set_to_gaussian():
	for i in range(N+1):
		var x = x_min + i*step;
		y += [amplitude*exp(-x*x/2)]
	pass



