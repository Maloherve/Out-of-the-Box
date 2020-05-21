shader_type canvas_item;

uniform sampler2D x_values;
uniform sampler2D y_values;
uniform int size;

const float scale = 1.7;

uniform float brightness = 0.004; 
uniform float thickness = 0.002;
uniform vec4 color_neon : hint_color = vec4(vec3(0.8, 0.2, 0.05), 1.);

const float PI = 3.14;


vec2 get_position(ivec2 coord)
{
	// retrieve r component on the desired array position
	float x = texelFetch(x_values, coord, 0).r;
	float y = texelFetch(y_values, coord, 0).r;
	// the resulting value is in srgb space, so convert it back to linear space
	return vec2(x, y);
}


//Distance to a line segment,
float dfLine(vec2 start, vec2 end, vec2 uv){
	start = vec2(start.x * scale, start.y);
	end = vec2(end.x * scale, end.y);
	
	vec2 line = end - start;
	float frac = dot(uv - start, line) / dot(line,line);
	
	return distance(start + line * clamp(frac, 0f, 1f), uv);
}

//Distance to an arc.
float dfArc(vec2 origin, float start, float sweep, float radius, vec2 uv){
	uv -= vec2(origin.x * scale , origin.y);
	uv *= mat2(vec2(cos(start), sin(start)), vec2(-sin(start), cos(start)));
	float offs = (sweep / 2.0 - 3.14);
	float angle = mod( atan(uv.y/abs(uv.x)) - offs, 6.28 ) + offs;
	angle = clamp(angle, min(0, sweep), max(0.0, sweep));
	return distance(radius * vec2(cos(angle), sin(angle)), uv);
}



float shape(vec2 uv, float infinity){
	float dist = infinity;
	
	
//	vec2 point1 = get_position(ivec2(0,0));
//	vec2 point2 = get_position(ivec2(1,0));
//	dist = min(dist, dfLine(point1, point2, uv) );
	
	vec2 point1;
	vec2 point2 = vec2(0.,0.);
	for (int i=0; i < size - 1; i++){
		point1 = get_position(ivec2(i, 0));
		point2 = get_position(ivec2(i+1, 0));
		dist = min(dist, dfLine(point1, point2, uv) );
	}
	dist = min(dist, dfLine(point2, get_position(ivec2(0, 0)), uv) );
	
	
	
//	// Box
//	dist = min(dist, dfLine(vec2(.25, .25), vec2(.25, .75), uv) );
//	dist = min(dist, dfLine(vec2(.25, .75), vec2(.75, .75), uv) );
//	dist = min(dist, dfLine(vec2(.75, .75), vec2(.75, .25), uv) );
//	dist = min(dist, dfLine(vec2(.75, .25), vec2(.25, .25), uv) );

	
//	// T
//	dist = min(dist, dfLine(vec2(.275, .35), vec2(.45, .35), uv) );
//	dist = min(dist, dfLine(vec2(.3625, .35), vec2(.3625, .65), uv) );
//
//	// U 
//	dist = min(dist, dfArc(vec2(0.5, 0.5), 0f, PI/2., .1, uv) );
//	dist = min(dist, dfArc(vec2(0.5, 0.5), PI/2., PI/2., .1, uv) );
//
//	// T
//	dist = min(dist, dfLine(vec2(.55, .35), vec2(.725, .35), uv) );
//	dist = min(dist, dfLine(vec2(.6375, .35), vec2(.6375, .65), uv) );

//	dist = min(dist, dfLine(varvec1, varvec2, uv)) ;
//	dist = min(dist, dfLine(varvec2, varvec3, uv)) ;

	
	return dist; 
}


float norm(vec3 vec){
	float r = vec.r;
	float g = vec.g;
	float b = vec.b;
	
	return sqrt(r*r + g*g + b*b);
}


void fragment(){
	
	vec2 coord = vec2(UV.x * scale, UV.y);
	
	float dist; float shade;
	// Random brihghtness
	float bright = brightness * min(1.0, 1.0 - sin(max(1e-3, abs(sin(121.0*TIME))) * 150.0));
	
	dist = shape(coord, 1e6); // Distance from the closest line / arc
	shade = bright / max(1e-3, dist-thickness);
	
	vec4 color = vec4(color_neon.rgb * shade, 1.);
	color.a = float( norm(color.rgb) > 0.6); // Make bg transparent
	COLOR = color;
}