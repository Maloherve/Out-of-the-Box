tool
extends Area2D

var detector_index : int = -1;

const base_material = preload('res://assets/Particles/detector.tres');
var emitter : ParticlesMaterial;

export (Color) var rest_color = Color(1.0, 1.0, 1.0, 1.0);
export (Color) var activated_color = Color(1.0, 0.5, 0.2, 1.0);

func detect(system):	
	#var T = get_relative_transform_to_parent(system);
	var T = system.get_global_transform().affine_inverse() * get_global_transform();
	# get extrema in the system coordinates
	#var x_beg = system.get_global_transform().xform_inv(get_global_transform().xform(Vector2(-$domain.shape.extents.x, 0))).x;
	#var x_end = system.get_global_transform().xform_inv(get_global_transform().xform(Vector2( $domain.shape.extents.x, 0))).x;
	var x_beg = T.xform(Vector2(-$domain.shape.extents.x, 0)).x;
	var x_end = T.xform(Vector2( $domain.shape.extents.x, 0)).x;
	return system.probability(x_beg, x_end);

func _init():
	emitter = base_material.duplicate();
	#connect("area_entered", self, "_on_Simulation_entered");
	#connect("area_exited", self, "_on_Simulation_exited");
		
func rescale():
	pass
	#$domain.shape.extents *= global_scale
	#scale /= global_scale
	
# automatic rescale only during initialization
func _ready():
	rescale()
	# make this material unique
	$stars.process_material = emitter;
	emitter.color = rest_color;


func _on_detector_area_entered(area):
	$stars.amount = 8;
	emitter.color = activated_color;
	emitter.initial_velocity *= 1.5;
	pass # Replace with function body.


func _on_detector_area_exited(area):
	$stars.amount = 3;
	emitter.color = rest_color;
	emitter.initial_velocity /= 1.5;
	pass # Replace with function body.
