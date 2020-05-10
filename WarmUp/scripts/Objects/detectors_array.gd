extends Node2D

const detc_scene = preload("res://scenes/Wave/detector.tscn");
var detc = [];

# dynamic detector placer

func detectors_nb(nb):
	return detc.size();
	
func add_detector(location = Vector2(0,0), dscale = Vector2(1,1), children = []):
	var d = detc_scene.new();
	d.position = location;
	d.scale = dscale;
	for child in children:
		d.add_child(child);
	detc.push_back(d);
	add_child(d);
	return d;
	
# get a reference to a specific detector
func get_detector(index):
	return detc[index];
	
# add new detectors with a constant spacing
# nb is an int
# disposition is a Curve2D
# scale_fct is a Curve or a Vector2
func linear_dispose(nb, disposition, scale_fct = Vector2(1,1), children = []):
	for i in range(nb):
		var t = float(i) / (nb-1);
		var pos = disposition.interpolate_baked(t);
		var dscale = scale_fct;
		if scale_fct is Curve:
			dscale = scale_fct.interpolate_baked(t);			
		add_detector(pos, dscale, children);
