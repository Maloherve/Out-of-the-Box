shader_type canvas_item;

// noise coordinates
uniform float amplitude = 0.2;
uniform float half_width = 0.05;
uniform vec3 theme = vec3(1.0, 0.0, 1.0);

// curve deformation

// hw = half width
float plot(vec2 st, float pct, float hw) {
	return smoothstep(pct - hw, pct, st.y) - smoothstep(pct, pct + hw, st.y);
}

float rand(vec2 n) {
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float noise(vec2 n) {
	vec2 d = vec2(0.0, 1.0);
	vec2 b = floor(n);
	vec2 f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
	return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}

float fbm(vec2 n) {
    float total = 0.0;
	float ampli = 1.0;
    for (int i = 0; i < 7; i++) {
        total += noise(n) * ampli;
        n += n;
        ampli *= 0.7;
    }
    return total;
}

void fragment() {
	
	// noise
	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

	vec2 t = UV * vec2(2.0, 1.0) - TIME * 3.0;
	float y = fbm(t) / 2.0;
	y = 0.4 + amplitude * (y - 0.5);
	float pct = plot(UV, y, half_width);
	color += pct * vec4(1.0, 1.0, 1.0, 1.0);
	color.rgb += theme.rgb;
	
	// curve plot
	
	// apply changes
	COLOR = color;
	
}