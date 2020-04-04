#include "qgridsystem1D.hpp"

using namespace qgdn;

void qgridsystem1D::_register_methods() {
    // direct methods
    register_property<qgridsystem1D, double>("set_mass", &qgridsystem1D::set_mass, &qgridsystem1D::mass);
    register_method("energy", &qgridsystem1D::energy);
    register_method("position", &qgridsystem1D::position);
    register_method("momentum", &qgridsystem1D::momentum);
    register_method("normalize", &qgridsystem1D::normalize);
    register_method("map", &qgridsystem1D::map);
    register_method("size", &qgridsystem1D::size);
    
    // TODO, methods that need to be adapted
    //register_method("psi", &qgridsystem1D::gdn_psi);
    register_method("V", &qgridsystem1D::set_potential, &qgridsystem1D::get_potential);
}


void set_potential(godot::Reference* pot) {
    auto ptr = dynamic_cast<qsim::potential<size_t>*>(pot);

    if (ptr == nullptr) {
        // TODO throw godot exception
        return;
    }

    if(!pot->reference()) {
        // TODO cannot bind reference
        return;
    }

    if (m_potential != nullptr && !m_potential->unreference()) {
        // TODO cannot unreference old potential
        return;
    }
    
    // set the old potential 
    m_potential = pot; 
    
    // finally set the potential 
    qsim::grid::qsystem1D::set_potential(ptr); 
}

void set_evolver(godot::Reference* evo) {
    auto ptr = dynamic_cast<qsim::grid::qsystem1D::evolver*>(evo);

    if (ptr == nullptr) {
        // TODO throw godot exception
        return;
    }

    if(!evo->reference()) {
        // TODO cannot bind reference
        return;
    }

    if (m_evolver != nullptr && !m_evolver->unreference()) {
        // TODO cannot unreference old potential
        return;
    }
    
    // set the old potential 
    m_evolver = evo; 
    
    // finally set the potential 
    qsim::grid::qsystem1D::set_evolver(ptr);
}

Reference * get_potential() const {
    return m_potential;
}

Reference * get_evolver() const {
    return m_evolver;
}

void qgridsystem1D::_init() {
    // default values
}

void qgridsystem1D::_process(double dt) {
    // default values
    sim::grid::qsystem1D::evolve(dt);
}
