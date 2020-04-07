extends CanvasModulate


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var high_color = Color(255, 12, 223, 255) / 255
var low_color = Color(78, 48, 74, 255) / 255
var frequency = 0.3

#func _init():
	

# Called when the node enters the scene tree for the first time.
func _ready():
	self.color = low_color
	set_process(true)
	
var t = 0

func color_loc(y):
	return high_color * y + low_color * (1.0-y)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var y = abs(sin(PI * frequency * t))
	self.color = color_loc(y)
	t += delta
	if t > 2.0/frequency:
		t -= 2.0 /frequency
