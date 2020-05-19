extends Sprite

export (Curve) var fader_in;
export (Curve) var fader_out;
export (Curve) var fader_in_out;

enum fmode {
	enter,
	exit,
	enter_and_exit
};

var mode = fmode.enter;
var tick : float = 0.0;
var total_time = 1.0;

var delayer : Timer = Timer.new();

func fade_in(sec):
	set_process(true);
	tick = sec;
	mode = fmode.enter;
	total_time = sec;
	modulate.a = 0.0;
	visible = true;
	
func fade_out(sec):
	set_process(true);
	tick = sec;
	mode = fmode.exit;
	total_time = sec;
	
func fade_in_out(sec, delay = 0.0):
	mode = fmode.enter_and_exit;
	tick = sec;
	total_time = sec;
	modulate.a = 0.0;
	visible = true;
	if delay > 0:
		print("With delay: ", delay)
		delayer.start(delay);
	else:
		set_process(true);
		
func _on_Timer_timeout():
	set_process(true);
	#remove_child(delayer);
		
func _init():
	delayer.one_shot = true;
	add_child(delayer);
	delayer.connect("timeout", self, "_on_Timer_timeout");
	set_process(false);

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var t = 0;
	match mode:
		fmode.enter:
			t = fader_in.interpolate_baked(1 - tick / total_time);
		fmode.exit:
			t = fader_out.interpolate_baked(1 - tick / total_time);
		fmode.enter_and_exit:
			t = fader_in_out.interpolate_baked(1 - tick / total_time);
		
	modulate.a = 255 * t;
	
	tick -= delta;
		
	if tick < 0:
		tick = 0.0;
		set_process(false);
		if mode != fmode.enter:
			visible = false; # optimise
