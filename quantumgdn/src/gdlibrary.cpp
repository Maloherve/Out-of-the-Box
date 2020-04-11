#include <Godot.hpp>
#include <Node.hpp>

#include "potential.hpp"
#include "uniform_potential.hpp"
#include "grid_wave.hpp"
#include "qsimbox.hpp"
#include "qgridsystem1D.hpp"
//#include "qgridsystem2D.hpp"


using namespace godot;

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o)
{
	Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o)
{
	Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle)
{
	Godot::nativescript_init(handle);
    
    // potentials
	register_class<grid_potential1D>();
	//register_class<grid_potential2D>();
	register_class<grid_uniform_potential1D>();
	//register_class<grid_uniform_potential2D>();

    // grid wave
	register_class<grid_wave>();

    // 1D initializers
	register_class<wave_init1D>();
	register_class<gauss_init1D>();

    // 2D initializers
	//register_class<wave_init2D>();
	//register_class<gauss_init2D>();

    // layers
    register_class<qsimbox>();
    
    // systems
	register_class<qgridsystem1D_base>();
	register_class<qgridsystem1D>();
	//register_class<qgridsystem2D>();
}

