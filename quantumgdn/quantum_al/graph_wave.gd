extends Polygon2D

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var sim = null
export (int) var resolution = 32
export (float) var gain = 30.0

var width = 1
var height = 1
var tex_off = Vector2(0,0);

func _init():
	if texture == null:
		push_error('Setup a f***ing texture')
	width = texture.get_width() * 2
	height = texture.get_height() * 2
	tex_off.x = -width/2
	tex_off.y = -height/2
	
func x(k):
	return tex_off.x + width * float(k) / resolution

# Called when the node enters the scene tree for the first time.
func _ready():
	# get parent instance
	sim = get_parent()
	
	set_process(false)
	
	# bake vertices array
	var array = []
	for k in range(resolution+1):
		array.push_back((Vector2(x(k), tex_off.y)))
	for k in range(resolution,-1,-1):
		array.push_back((Vector2(x(k), tex_off.y + height)))
		
	# set polygon vertices
	set_polygon(PoolVector2Array(array))
	
# wave function normalized 
func psi(k):
	var t = float(k) / resolution
	return sim.probability(t) * height * gain / 2

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var polyg = get_polygon()
	for k in range(1,resolution):
		var y = psi(k)
		print('k = ', k, '  y = ', y)
		polyg.set(k, Vector2(x(k), tex_off.y + psi(k)))
		polyg.set(polyg.size()-1-k, Vector2(x(k), tex_off.y + psi(k) + height))
	set_polygon(polyg)
