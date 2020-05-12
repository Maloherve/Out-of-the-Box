extends Area2D

export (Color) var color = Color(1.0, 1.0, 1.0, 1.0);

export (float) var fade_in_time = 0.0;
export (Curve) var fade_in_curve;

export (float) var fade_out_time = 0.0;
export (Curve) var fade_out_curve;

var old_color = Color(1.0, 1.0, 1.0, 1.0);
var body = null;
var tick = 0.0;

func _on_Area2D_Body_entered(node):
	var parent = get_parent();
	if body == null && parent != null && parent is CanvasItem:
		# reset if necessary
		if is_processing():
			reset_tick();
		# set parent modulation
		old_color = parent.modulate;
		if fade_in_time <= 0:
			parent.modulate = color;
		else:
			set_process(true);
		body = node;
	
func _on_Area2D_Body_exited(node):
	var parent = get_parent();
	if body == node && parent != null && parent is CanvasItem:
		# restore old_color
		if fade_out_time <= 0:
			parent.modulate = old_color;
		else:
			set_process(true);
		body = null;
	
func _init():
	connect("body_entered", self, "_on_Area2D_Body_entered");
	connect("body_exited", self, "_on_Area2D_Body_exited");
	set_process(false);
	
func reset_tick():
	tick = 0.0;
	set_process(false);

func _process(delta):
	var t;
	var mod;
	if body == null:
		# fade out
		t = tick / fade_in_time;
		mod = color * fade_in_curve.interpolate_baked(1.0-t) + old_color * fade_in_curve.interpolate_baked(t);
	else:
		# fade in
		t = tick / fade_out_time;
		mod = color * fade_in_curve.interpolate_baked(t) + old_color * fade_in_curve.interpolate_baked(1.0-t);
		
	get_parent().modulate = mod;
	tick += delta;
	
	if t >= 1:
		reset_tick();
		
