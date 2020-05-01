shader_type canvas_item;
render_mode unshaded;

void vertex() {
	VERTEX.y += cos(VERTEX.x);
}