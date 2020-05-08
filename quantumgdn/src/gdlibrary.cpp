#include <Godot.hpp>
#include <Node.hpp>

//#include "potential.hpp"
//#include "uniform_potential.hpp"
//#include "composed_potential.hpp"
#include "potential_field.hpp"
#include "curve_field.hpp"
#include "curve_potential.hpp"
#include "grid_wave.hpp"
#include "qgridsystem1D.hpp"
//#include "qgridsystem2D.hpp"


extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o)
{
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o)
{
	godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle)
{
	godot::Godot::nativescript_init(handle);
    
    // potentials 1D and super-classes
	//register_class<curve_potential>();

	//register_class<grid_composed_potential1D>();

	//register_class<grid_uniform_potential1D>();

	//register_class<grid_potential2D>();
	//register_class<grid_uniform_potential2D>();

    // potential fields
	godot::register_class<godot::potential_field>();
	godot::register_class<godot::curve_field>();

    // grid wave
	godot::register_class<godot::grid_wave>();
	godot::register_class<godot::grid_wave1D>();

    // 1D initializers
	godot::register_class<godot::wave_init1D>();
	godot::register_class<godot::gauss_init1D>();

    // 2D initializers
	//register_class<wave_init2D>();
	//register_class<gauss_init2D>();

    // layers
    //register_class<qsimbox>();
    
    // systems
    godot::register_class<godot::gdqsystem>();
	godot::register_class<godot::qgridsystem1D>();
    
	//register_class<qgridsystem2D>();
}

