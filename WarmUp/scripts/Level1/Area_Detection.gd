extends Area2D



# Called when the node enters the scene tree for the first time.
func _ready():
	self.connect("body_entered", self, "_on_body_entered");


func _on_body_entered(body):
	if body.name == "Player":
		get_tree().get_root().get_node("Level/Player").set_locked(true);
		
		var WaitTimer1 = Timer.new(); WaitTimer1.name = "WaitTimer1";
		add_child(WaitTimer1)
		WaitTimer1.wait_time = 1.0;
		WaitTimer1.start();
		WaitTimer1.connect("timeout", self, "_on_WaitTimer1_timeout");


func _on_WaitTimer1_timeout():
	$WaitTimer1.queue_free();
	get_tree().get_root().get_node("Level/World/Enemies/Scientist2").flip_h = true;
	var WaitTimer2 = Timer.new(); WaitTimer2.name = "WaitTimer1";
	add_child(WaitTimer2)
	WaitTimer2.wait_time = 2.0;
	WaitTimer2.start();
	WaitTimer2.connect("timeout", self, "_on_WaitTimer2_timeout");


func _on_WaitTimer2_timeout():
	get_tree().get_root().get_node("Level/Player").die("Detected");
