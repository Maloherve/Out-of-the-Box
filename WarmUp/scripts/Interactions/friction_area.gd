extends Area2D

var bodies : Array = [];
var endurance_fall : Array = [];

export (float) var friction = 0.0;
export (float) var upper_friction = 0.0;
export (float) var cascade = 0.0;

export (int) var endurance_decrease = 40;

func _on_body_entered(body):
	if body.get("TYPE") == "PLAYER":
		bodies.append(body);
		endurance_fall.append(body.get_node("Endurance").add_increase_process(1.5));
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
			body.get_node("Endurance").rm_increase_process(endurance_fall[index]);
			endurance_fall.remove(index);
			bodies.remove(index);
			if bodies.empty():
				set_physics_process(false);
	

func _init():
	connect("body_entered", self, "_on_body_entered")
	connect("body_exited", self, "_on_body_exited")
	#set_process(false);
	set_physics_process(false);
	
func _process(_delta):
	if Input.is_action_just_pressed("ui_up"):
		for index in range(bodies.size()):
			var body = bodies[index];
			if body.has_endurance():
				body.land();
				body.get_node("Jumper").jump_one_shot();
				#body.get_node("Endurance").decrease(endurance_decrease);
				if !body.has_endurance():
					body.get_node("Endurance").rm_increase_process(endurance_fall[index]);
					endurance_fall[index] = null;

# Called when the node enters the scene tree for the first time.
func _physics_process(delta):
	for body in bodies:
		if body.velocity.y < 0:
			body.velocity.y *= (1.0 - upper_friction);
		else:
			body.velocity.y *= (1.0 - friction);
		body.velocity.x *= 1.0 - friction;
		body.velocity.y += cascade;

