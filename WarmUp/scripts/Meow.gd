extends AudioStreamPlayer


const tracks = [
  'Cat_Meow_1',
  'Cat_Meow_2',
  'Cat_Meow_3',
  'Cat_Meow_4'
]

func _ready():
	pass
	

func play_random_meow():
	var rand_nb = randi() % tracks.size()
	var audiostream = load("res://assets/Sounds/Meow/" + tracks[rand_nb] + '.wav')
	set_stream(audiostream)
	play()

func _on_Timer_timeout():
	$Timer.wait_time = (randi() % 15) + 5 
	play_random_meow()
