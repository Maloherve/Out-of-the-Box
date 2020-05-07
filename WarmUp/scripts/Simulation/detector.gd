tool
extends Area2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var detector_index : int = -1;

func detect(system):	
	#var T = get_relative_transform_to_parent(system);
	var T = system.get_global_transform().affine_inverse() * get_global_transform();
	# get extrema in the system coordinates
	#var x_beg = system.get_global_transform().xform_inv(get_global_transform().xform(Vector2(-$domain.shape.extents.x, 0))).x;
	#var x_end = system.get_global_transform().xform_inv(get_global_transform().xform(Vector2( $domain.shape.extents.x, 0))).x;
	var x_beg = T.xform(Vector2(-$domain.shape.extents.x, 0)).x;
	var x_end = T.xform(Vector2( $domain.shape.extents.x, 0)).x;
	return system.probability(x_beg, x_end);
	
func rescale():
	pass
	#$domain.shape.extents *= global_scale
	#scale /= global_scale

# automatic rescale only during initialization
func _ready():
	rescale()
	
