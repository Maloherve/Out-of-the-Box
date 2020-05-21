extends Resource

export (AudioStream) var soundtrack;
export (Color) var modulate;

func _init(p_soundtrack = null, p_modulate = Color(1,1,1,1)):
	soundtrack = p_soundtrack;
	modulate = p_modulate;

