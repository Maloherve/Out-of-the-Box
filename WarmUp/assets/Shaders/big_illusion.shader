shader_type canvas_item;

uniform vec2 tiled_factor = vec2(1.0, 0.8); 
uniform vec2 modulation_offset = vec2(0.4, 0.25);
uniform float oscillation_norm = 0.05;
//uniform vec3 base_modulation = vec3(1.0, 0.9, 0.)

void fragment() {
	vec2 tiled_uvs = UV * tiled_factor;
	vec2 oscillation = UV;
	oscillation.x *= cos(TIME) * oscillation_norm;
	oscillation.y *= sin(TIME) * oscillation_norm;
	vec2 modulation = UV * cos(TIME) * 0.2;
	float redmod = sin(2.0 * TIME) * 0.1;
	COLOR = texture(TEXTURE, tiled_uvs + oscillation, 1.0);
	COLOR.gb *= modulation_offset - modulation;
	COLOR.r *= 0.8 + redmod;
}