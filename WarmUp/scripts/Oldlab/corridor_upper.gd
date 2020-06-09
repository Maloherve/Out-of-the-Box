extends TileMap

export (bool) var value_start = true;

onready var boss = get_tree().get_current_scene().get_node("boss");
onready var triggerer = get_parent().get_node("Interactions/boss_trigger");

# Called when the node enters the scene tree for the first time.
func _ready():
	boss.connect("starting", self, "_on_boss_starting");

func _on_boss_starting():
	visible = value_start;
	set_collision_layer_bit(1, value_start);
	set_collision_layer_bit(5, value_start);
	if value_start:
		modulate *= triggerer.MODULATOR;

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
