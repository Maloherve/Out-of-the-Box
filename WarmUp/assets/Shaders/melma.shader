shader_type canvas_item;

uniform float transparency = 0.9;
uniform vec2 tiled_factor = vec2(1.0, 0.5);
uniform float aspect_ratio = 1.0;

uniform vec3 modulation = vec3(1.0, 1.0, 0.1);

void fragment() {
	vec2 tiled_uvs = UV * tiled_factor;
	tiled_uvs.y *= aspect_ratio;
	
	vec2 wave_uv_offset;
	wave_uv_offset.x = cos(TIME + tiled_uvs.x + tiled_uvs.y) * 0.5;
	wave_uv_offset.y = -TIME * 0.5;
	
	COLOR = texture(TEXTURE, tiled_uvs + wave_uv_offset);
	COLOR.a = transparency;
	COLOR.rgb *= modulation;
}