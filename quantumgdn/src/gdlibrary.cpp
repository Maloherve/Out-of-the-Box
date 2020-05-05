#include <Godot.hpp>
#include <Node.hpp>

#include "potential_field.hpp"
#include "curve_field.hpp"
#include "uniform_field.hpp"
#include "curve_potential.hpp"
#include "grid_wave.hpp"
#include "qgridsystem1D.hpp"


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

    // potential fields
	godot::register_class<godot::potential_field>();
	godot::register_class<godot::curve_field>();
	godot::register_class<godot::uniform_field>();

    // grid wave
	godot::register_class<godot::grid_wave>();
	godot::register_class<godot::grid_wave1D>();

    // 1D initializers
	godot::register_class<godot::wave_init1D>();
	godot::register_class<godot::gauss_init1D>();

    // 2D initializers
	//register_class<wave_init2D>();
	//register_class<gauss_init2D>();

    // systems
    godot::register_class<godot::gdqsystem>();
	godot::register_class<godot::qgridsystem1D>();
    
	//register_class<qgridsystem2D>();
}

