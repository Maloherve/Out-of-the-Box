extends AudioStreamPlayer

var queue : Array = [];

export (float) var fadein_time = 0;
export (float) var fadeout_time = 0;
export (Curve) var fadein;
export (Curve) var fadeout;

var tick : float = 0;
var finishing = false;

func _on_AudioStreamPlayer_finished():
	if !queue.empty():
		if fadeout_time > 0:
			set_process(true);
			tick = fadeout_time;
			finishing = true;
	
func _init():
	connect("finished", self, "_on_AudioStreamPlayer_finished");
	set_process(false);
	
func push_track(track):
	track.loop = true;
	queue.append(track);
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if finishing:
		var t = fadeout.interpolate_baked(tick / fadeout_time);
		self.volume_db =  -80 * t;
	else:
		var t = fadein.interpolate_baked(1 - tick / fadein_time);
		self.volume_db =  -80 * (1-t);

	tick -= delta;
	
	if tick < 0:
		if finishing:
			finishing = false;
			self.stream = queue.front();
			queue.pop_front();
			if fadein_time > 0:
				tick = fadein_time;
			else:
				tick = 0;
				set_process(false);
		else:
			tick = 0;
			set_process(false);