extends Line2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

export (float) var gain = 1
export (int) var resolution = 16
var curve = Curve.new()
onready var sim = get_parent()

func plot(t):
	return sim.V(t) * gain
	
func reset():
	curve.clear_points()
	curve.add_point(Vector2(0, 0))
	for k in range(1,resolution):
		var t = float(k)/resolution
		curve.add_point(Vector2(t, plot(k)))
	curve.add_point(Vector2(1, 0))
	
	curve.bake()
	for k in range(0,resolution+1):
		var t = float(k)/resolution
		#print("Called psi(t): ",  curve.interpolate_baked(t))
		add_point(Vector2(sim.x(t), curve.interpolate_baked(t)))

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	for k in range(1,resolution):
		var t = float(k)/resolution
		curve.set_point_value(k,plot(t))
		
	curve.bake()
		
	for k in range(1,resolution):
		var t = float(k)/resolution
		#print("Called t = ", t, ", psi(t): ",  Vector2(sim.x(t), curve.interpolate_baked(t)))
		set_point_position(k, Vector2(sim.x(t), curve.interpolate_baked(t)))
		
