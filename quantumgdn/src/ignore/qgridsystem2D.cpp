#include "qgridsystem2D.hpp"

qgridsystem2D::qgridsystem2D() 
    : qsystem2D(1.0, 1, 1, 
                std::make_shared<qsim::pot::uniform<size_t>>(),
                gauss_pack(),
                std::make_shared<qsystem1D::explicit_evolver>()
            )
{

}

qgridsystem2D::~qgridsystem2D() {
}
        
Vector2 qgridsystem2D::position() const {
    auto pos = qsystem2D::position();
    return Vector2(pos.first, pos.second);
}

Vector2 qgridsystem2D::momentum() const {
    auto mom = qsystem2D::momentum();
    return Vector2(mom.first, mom.second);
}

void qgridsystem2D::set_wave(Ref<wave_init2D> init) {
    if (init != nullptr)
        replace_wave(*init);
}

void qgridsystem2D::_init() {
}

void qgridsystem2D::_ready() {
    npdebug("Looking for a potential")
    // look for a potential
    for (auto * child : get_children()) {
        grid_potential * pot = dynamic_cast<grid_potential>(child);

        if (pot != nullptr && pot->is_safe()) {
            qsim::grid::qsystem2D::set_potential(*(*pot));
            break; // found
        }
    }
}
        
void qgridsystem2D::_fixed_process(double dt) {
    evolve(dt);
}

void qgridsystem2D::_register_methods() {
    // godot built-in
    register_method("_physics_process", &qgridsystem2D::_fixed_process);
    register_method("_ready", &qgridsystem2D::_ready);

    // direct methods
    register_method("energy", &qgridsystem2D::_energy);
    register_method("position", &qgridsystem2D::_position);
    register_method("momentum", &qgridsystem2D::_momentum);
    register_method("set_wave", &qgridsystem2D::_set_wave);
    
    // properties
    register_property<qgridsystem2D, double>("mass", &qgridsystem2D::set_mass, &qgridsystem2D::mass, 1.0);
    register_property<qgridsystem2D, double>("hbar", &qgridsystem2D::set_hbar, &qgridsystem2D::get_hbar, 1.0);
    //register_property<qgridsystem2D, grid_potential*>("V", &qgridsystem2D::_set_potential, &qgridsystem2D::_get_potential, nullptr);
}
