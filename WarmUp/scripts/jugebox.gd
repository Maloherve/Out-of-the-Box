extends AudioStreamPlayer

var queue = null;

export (float) var max_volume = 0;

export (float) var fadein_time = 0;
export (float) var fadeout_time = 0;
export (Curve) var fadein;
export (Curve) var fadeout;

var tick : float = 0;
var finishing = false;

func change():
	#if queue != null:
	var t = fadein.interpolate_baked(0);
	self.volume_db = -80 * (1 - t) + max_volume * t;
		
	if fadeout_time > 0 && self.stream != null:
		tick = fadeout_time;
		finishing = true;
	else:
		tick = fadein_time;
		finishing = false;
		self.stream = queue;
		queue = null;
		play();
			
	set_process(true);
	
func _init():
	#connect("finished", self, "_on_AudioStreamPlayer_finished");
	set_process(false);
	
	
func push_track(track):
	if track != null:
		track.loop = true;
	queue = track;
	change() # trigger beginning
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if finishing:
		var t = fadeout.interpolate_baked(1 - tick / fadeout_time);
		self.volume_db =  -80 * (1-t) + max_volume * t;
	else:
		var t = fadein.interpolate_baked(1 - tick / fadein_time);
		self.volume_db =  -80 * (1-t) + max_volume * t;

	tick -= delta;
	
	if tick < 0:
		if finishing: # fade out end
			self.stream = queue;
			finishing = false;
			queue = null;
			play();
			if fadein_time > 0:
				tick = fadein_time;
			else:
				tick = 0;
				set_process(false);
		else: # fade in end
			tick = 0;
			set_process(false);
