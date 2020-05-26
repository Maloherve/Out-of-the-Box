extends Area2D

var bodies : Array = [];
export (float) var friction = 0.0;
export (float) var upper_friction = 0.0;
export (float) var cascade = 0.0;

func _on_body_entered(body):
	if body.get("velocity"):
		bodies.append(body);
		if !is_physics_processing():
			set_physics_process(true);
	elif body is RigidBody2D:
		body.friction = friction;
	
func _on_body_exited(body):
	if body is RigidBody2D:
		body.friction = 0.0;
	else:
		var index = bodies.find(body);
		if index != -1:
			bodies.remove(index);
			if bodies.empty():
				set_physics_process(false);
	

func _init():
	connect("body_entered", self, "_on_body_entered")
	connect("body_exited", self, "_on_body_exited")
	#set_process(false);
	set_physics_process(false);
	
func _process(_delta):
	for body in bodies:
		if body.has_node("Jumper") && Input.is_action_just_pressed("ui_up"):
			if body.has_endurance():
				body.land();
				body.get_node("Jumper").jump_one_shot();

# Called when the node enters the scene tree for the first time.
func _physics_process(delta):
	for body in bodies:
		if body.velocity.y < 0:
			body.velocity.y *= (1.0 - upper_friction);
		else:
			body.velocity.y *= (1.0 - friction);
		body.velocity.x *= 1.0 - friction;
		body.velocity.y += cascade;

