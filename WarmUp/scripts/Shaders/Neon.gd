extends Sprite


onready var Polygon : Polygon2D = get_node("Polygon2D");
onready var polygon_list : Array = Polygon.polygon;
onready var shader = get_material();


func _ready():
	var size : Vector2 = texture.get_size()

	# Get info from polygon2D, project it on [0,1], change the origin
	for i in range(polygon_list.size()):
		polygon_list[i] = Vector2(polygon_list[i].x / size.x, polygon_list[i].y / size.y);
		polygon_list[i] += Vector2(0.5, 0.5)
	save_polygon();

	# Seperate into an x and y array
	var array_x : Array;
	var array_y : Array;
	for i in range(polygon_list.size()):
		array_x.append(polygon_list[i].x);
		array_y.append(polygon_list[i].y);

	# Create textures from the arrays to send them to the shader
	var texture_x : Texture = create_texture_from_array(array_x);
	var texture_y : Texture = create_texture_from_array(array_y);
	# Upload the texture to my shader
	material.set_shader_param("x_values", texture_x)
	material.set_shader_param("y_values", texture_y)
	material.set_shader_param("size", array_x.size())



# Saves the polygon in a .save file
func save_polygon():
	var save_game = File.new();
	save_game.open("res://assets/shaders/polygon.save", File.WRITE);
	save_game.store_string("Polygon 1 :");
	for i in range(polygon_list.size()):
		save_game.store_line(to_json(polygon_list[i]));
	save_game.close()

# Converts an array into a texture
func create_texture_from_array(array : Array) -> Texture:
	# Projects on [0, 255]
	for i in range(array.size()):
		array[i] *= 255.0;

	# Dimensions
	var array_width = array.size();
	var array_heigh = 1

	# The following is used to convert the array into a Texture
	var byte_array = PoolByteArray(array)
	var img = Image.new()

	# No mipmaps generated : use_mipmaps = false
	# 1 component per pixel (the corresponding array value) : Format = Image.FORMAT_R8
	img.create_from_data(array_width, array_heigh, false, Image.FORMAT_R8, byte_array)
	var texture = ImageTexture.new()

	# Override the default flag with 0 since I don't want texture repeat/filtering/mipmaps/etc
	texture.create_from_image(img, 0)
	return texture
