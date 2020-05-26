extends Node

export (int) var maximum = 100;
export (int) var increase_speed = 1;
export (int) var decrease_speed = 2;

var endurance : int = maximum;
var automatic_decrease : bool = false setget set_automatic_decrease;
var automatic_increase : bool = false setget set_automatic_increase;

signal zero_reached();
signal maximum_reached();

func reset():
	endurance = maximum;
	
func decrease(value):
	if endurance > 0:
		endurance -= value;
		if endurance <= 0:
			endurance=0;
			emit_signal("zero_reached");
		
func increase(value):
	if endurance < maximum:
		endurance += value;
		if endurance >= maximum:
			reset();
			emit_signal("maximum_reached")
		
func set_automatic_decrease(flag):
	set_process(flag || automatic_increase);
	automatic_decrease = flag;
	
func set_automatic_increase(flag):
	set_process(flag || automatic_decrease);
	automatic_increase = flag;

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	if automatic_decrease:
		decrease(decrease_speed);
	if automatic_increase:
		increase(increase_speed);
