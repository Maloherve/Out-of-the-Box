extends AudioStreamPlayer

export (float) var fadeout_time = 1.0;
export (float) var fadein_time = .01;

export (float) var max_volume = 0.0;

var queue : AudioStream = null;

# accessibilty
var semaphore : bool = false;

func fade_in():
	$fade_in.interpolate_property(self, "volume_db",
			-80, max_volume, fadein_time,
			Tween.TRANS_LINEAR, Tween.EASE_IN);
	$fade_in.start();
	
func fade_out():
	$fade_out.interpolate_property(self, "volume_db",
			max_volume, -80, fadeout_time,
			Tween.TRANS_LINEAR, Tween.EASE_OUT);
	$fade_out.start();
	
func _ready():
	$fade_out.connect("tween_all_completed", self, "_on_fadeout_completed");
	$fade_in.connect("tween_all_completed", self, "_on_fadein_completed");
	
func _on_fadeout_tree_entered():
	pass
	
func _on_fadein_tree_entered():
	pass
	
func _on_fadeout_completed():
	if self.stream != null:
		self.stop();
		
	self.stream = queue;
	volume_db = -80;
	
	if self.stream != null:
		self.stream.loop = true;
		self.play();
		fade_in();
	else:
		semaphore = false;
	
func _on_fadein_completed():
	semaphore = false;
	
func push_track(track, force = false):
	if !force && (semaphore || track == self.stream):
		return;
		
	queue = track;
	
	if !is_ready():
		yield($fade_in, "tree_entered");
		yield($fade_out, "tree_entered");
		
	semaphore = true;
	if self.stream != null:
		fade_out();
	else:
		_on_fadeout_completed();
	
func is_ready():
	return $fade_in.is_inside_tree() && $fade_out.is_inside_tree();
	
	
