extends AnimatedSprite

var queue = []
var locked = false

export (String) var default_animation = "_idle"

#func _init():
#	connect("animation_finished", self, "_on_AnimatedSprite_animation_finished")

func is_not_duplicate():
	return queue.empty() || (!queue.empty() && get_animation() != queue.back())
	
func call(anim, lock = false):
	#print("Receiving: ", anim)
	if !locked:
		play(anim)
		locked = lock;
		if lock:
			yield(self, "animation_finished")
			locked = false;
			
func _on_AnimatedSprite_animation_finished():
	locked = false
	if !queue.empty():
		call(queue.front(),true)
		queue.pop_front()
