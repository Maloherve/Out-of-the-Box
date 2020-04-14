extends ReferenceRect


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	var system = get_parent()
	print("Width: ", system.width)
	self.margin_left = system.position.x
	self.margin_top = system.position.y
	self.margin_right = system.width + system.position.x
	self.margin_bottom = system.height + system.position.y
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
