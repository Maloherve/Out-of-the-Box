shader_type canvas_item;

uniform vec2 tiled_factor = vec2(1.0, 0.25);
uniform vec4 modulate = vec4(0.2627, 0.43137, 0.3019, 1.0);

void fragment() {
	vec2 tiled_uvs = UV * tiled_factor;
	COLOR = texture(TEXTURE, tiled_uvs, 5.0) * modulate;
}
	