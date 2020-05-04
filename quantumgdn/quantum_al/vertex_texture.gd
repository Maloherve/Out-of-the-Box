# Same as a sprite, but produces more geometry to make vertex shaders look nicer

tool
extends Node2D

export(Texture) var texture setget set_texture, get_texture
export var resolution = 4 setget set_resolution, get_resolution
export var debug_draw = false setget set_debug_draw

export var gain = 30

var sim = null

func _ready():
	sim = get_parent()

func get_texture():
	return texture

func set_texture(tex):
	texture = tex
	update()


func get_resolution():
	return resolution

func set_resolution(res):
	if res < 1 or res > 32:
		return
	resolution = res
	update()


func set_debug_draw(dd):
	debug_draw = dd
	update()

func plot(k, height):
	if sim != null:
		return sim.probability((float(k) + 0.5)/ resolution) * gain * height / 2 
	else:
		return 0

func _draw():
	if texture == null:
		return
	var tex_size = texture.get_size()
	var div_size = tex_size / Vector2(resolution, 1)

	draw_set_transform(-tex_size/2.0, 0.0, Vector2(1,1))

	for y in range(0,1):
		for x in range(0, resolution):
			var pos = Vector2(x,y)*div_size
			# set shader y coordinate
			print('k = ', x, '  y = ', plot(x, tex_size.y))
			material.set_shader_param("deformation", plot(x, tex_size.y))
			# draw texture part 
			var r = Rect2(pos, div_size)
			draw_texture_rect_region(texture, r, r)

	if debug_draw and get_tree().is_editor_hint():
		# Debug draw
		for y in range(0, 2):
			for x in range(0, resolution):
				var pos = Vector2(x,y)
				var r = Rect2(pos*div_size, div_size)
				stroke_rect(r, Color(1,1,0, 0.5))


func stroke_rect(r, c):
	draw_line(r.pos, Vector2(r.pos.x, r.pos.y), c)
	draw_line(r.pos, Vector2(r.pos.x, r.end.y), c)
	draw_line(r.end, Vector2(r.end.x, r.pos.y), c)
	draw_line(r.end, Vector2(r.pos.x, r.end.y), c)
