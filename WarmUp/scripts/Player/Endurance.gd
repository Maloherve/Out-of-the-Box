extends Node

export (int) var maximum = 200;
#export (int) var increase_speed = 1;
#export (int) var decrease_speed = 1;

var endurance : int = maximum;

# string -> int
var increase_processes : Dictionary = {};
var decrease_processes : Dictionary = {};

var next_decrease_id = 0;
var next_increase_id = 0;

var values_to_reach : Array = [];

#var automatic_decrease : bool = false setget set_automatic_decrease;
#var automatic_increase : bool = false setget set_automatic_increase;

signal zero_reached();
signal maximum_reached();
signal value_reached(which, increasing);

func _init():
	set_process(true);

func reset():
	endurance = maximum;
	
func decrease(value):
	if endurance > 0:
		for val in values_to_reach:
			if (endurance - value) <= val && endurance > val:
				emit_signal("value_reached", val, false);
		endurance -= value;
		if endurance <= 0:
			endurance=0;
			emit_signal("zero_reached");
		
func increase(value):
	if endurance < maximum:
		for val in values_to_reach:
			if (endurance + value) >= val && endurance < val:
				emit_signal("value_reached", val, true);
		endurance += value;
		if endurance >= maximum:
			reset();
			emit_signal("maximum_reached")
		
func add_decrease_process(value):
	next_decrease_id += 1;
	var id = next_decrease_id;
	decrease_processes[id] = value;
	return id;
	
func add_increase_process(value):
	next_increase_id += 1;
	var id = next_increase_id;
	increase_processes[id] = value;
	return id;
	
func rm_decrease_process(id):
	if id != null:
		decrease_processes.erase(id);
		if decrease_processes.empty():
			next_decrease_id = 0;
	
func rm_increase_process(id):
	if id != null:
		increase_processes.erase(id);
		if increase_processes.empty():
			next_increase_id = 0;

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	for value in decrease_processes.values():
		#print("DECREASING: ", -value)
		decrease(value);
	for value in increase_processes.values():
		#print("INCREASING: ", value)
		increase(value);
