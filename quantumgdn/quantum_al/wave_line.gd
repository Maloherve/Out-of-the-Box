extends Line2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var sim = null
export var gain = 30.0
export (int) var resolution = 16
var curve = Curve.new()

#var curve = Curve2D.new()

func plot(t):
	return sim.probability(t) * gain * sim.height / 2
	
# Called when the node enters the scene tree for the first time.
func _ready():
	sim = get_parent()
	
func reset():
	curve.clear_points()
	#add_point(plot(-1))
	for k in range(0,resolution):
		var t = float(k)/resolution
		curve.add_point(Vector2(t, plot(k)))
		if k == resolution:
			print(sim.x(k))
		add_point(Vector2(sim.x(k), 0))
	#add_point(plot(resolution))


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	for k in range(0,resolution):
		#evaluate second derivative
		var t = float(k)/resolution
		if k == 0:
			print(sim.x(k))
		curve.set_point_value(k,plot(t))
		
	curve.bake()
		
	for k in range(0,resolution):
		var t = float(k)/resolution
		set_point_position(k, Vector2(sim.x(t), curve.interpolate_baked(t)))
		
