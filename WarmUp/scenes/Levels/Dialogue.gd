extends RichTextLabel


# Declare member variables here. Examples:
var dialog = [
	'So... How is it going with Experiment 8?', 
	'Any progress?',
	'None so far... We had high hopes for Schrödy- hum, I mean Experiment 8.',
	'*insert scientific explanations here*',
	'*insert scientific tests conducted*',
	'Alright, well, you better get some results and fast!', 
	'The president himself is counting on you.'
	]
var page = 0


# Called when the node enters the scene tree for the first time.
func _ready():
	pass
	
func _input(event):
	if event is InputEventMouseButton && event.is_pressed():
		if get_visible_characters() > get_total_character_count():
			if page < dialog.size()-1:
				page += 1
				set_bbcode(dialog[page])
				set_visible_characters(0)
		else:
			set_visible_characters(get_total_character_count())

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_Timer_timeout():
	set_visible_characters(get_visible_characters()+1)


func _on_Box_animation_finished():
	set_bbcode(dialog[page])
	set_visible_characters(0)
	set_process_input(true)
