extends Resource

export (AudioStream) var soundtrack;
export (Color) var modulate = Color(1,1,1,1);

func _init(p_soundtrack = null, p_modulate = null):
	soundtrack = p_soundtrack;
	modulate = p_modulate;

