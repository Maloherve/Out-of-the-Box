tool
extends HBoxContainer

export (float) var energy = 4.0 setget set_energy;
export (float) var energy_max = 20.0 setget set_energy_max;

onready var progress_mat : Material = $termometer/bar.material;

var ready = false;

func set_energy_max(value):
	if value > 0 && ready:
		$termometer/bar.max_value = value;
	energy_max = value;

func set_energy(value):
	if value > 0 && ready:
		$termometer/Value.text = str(floor(value));
		$termometer/bar.value = value;
		#lerp(color_min, color_max, value/energy_max)
		progress_mat.set_shader_param("modulation", value/energy_max);
	energy = value;
	
func connect_player(player):
	if player.get("TYPE") == "PLAYER":
		player.get_node("WaveCaster").connect("energy_changed", self, "_on_Player_energy_changed");
	
func _on_Player_energy_changed(value):
	set_energy(value);

# Called when the node enters the scene tree for the first time.
func _ready():
	ready = true;
	set_energy_max(energy_max);
	set_energy(energy);

