shader_type canvas_item;

uniform vec2 tiled_factor = vec2(1.0, 0.5);
uniform float aspect_ratio = 1.0;

uniform float modulation = 1.0;

uniform vec4 mod_max = vec4(5, 0.4, 0.1, 1.0);
uniform vec4 mod_min = vec4(1.0, 1.0, 1.0, 1.0);

// oscillation
uniform float lumin = 5;
uniform float lumin_T = 5;

void fragment() {
	vec2 tiled_uvs = UV * tiled_factor;
	tiled_uvs.y *= aspect_ratio;
	
	vec2 wave_uv_offset;
	wave_uv_offset.x = cos(TIME * 0.5 + tiled_uvs.x + tiled_uvs.y) * 0.3;
	wave_uv_offset.y = -TIME * 0.5;
	
	COLOR = texture(TEXTURE, tiled_uvs + wave_uv_offset);
	COLOR *= mod_max * modulation + mod_min * (1.0 - modulation);
}