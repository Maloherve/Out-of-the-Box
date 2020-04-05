#include "qgridsystem1D.hpp"

#include "quantumsim/potentials/uniform.hpp"
#include "quantumsim/evolvers/explicit.hpp"

using namespace godot;
using namespace qsim::grid;
using namespace qsim::pot;

qgridsystem1D::qgridsystem1D() 
    : qsystem1D(1.0, 
                {-1.0, 1.0},
                wave_vector(100, 0.0), // empty wave
                std::make_shared<uniform<size_t>>(), // zero potential, not a godot reference
                std::make_shared<qsystem1D::explicit_evolver>()),
        m_potential(nullptr) {}

qgridsystem1D::~qgridsystem1D() {}

void qgridsystem1D::_register_methods() {
    // direct methods
    register_method("_process", &qgridsystem1D::_process);
    register_property<qgridsystem1D, double>("mass", &qgridsystem1D::set_mass, &qgridsystem1D::mass, 1.0);
    //register_method("energy", &qgridsystem1D::energy);
    //register_method("position", &qgridsystem1D::position);
    //register_method("momentum", &qgridsystem1D::momentum);
    //register_method("normalize", &qgridsystem1D::normalize);
    //register_method("map", &qgridsystem1D::map);
    //register_method("size", &qgridsystem1D::size);
    
    // TODO, methods that need to be adapted
    //register_method("psi", &qgridsystem1D::gdn_psi);
    //register_property<qgridsystem1D, Reference*>("V", &qgridsystem1D::set_potential, &qgridsystem1D::get_potential);
}


void qgridsystem1D::set_potential(potential<size_t>* pot) {

    if (pot == nullptr) {
        // TODO throw godot exception
        return;
    }

    auto ptr = pot->get_ptr(); 

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

/*void set_evolver(Reference* evo) {
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
}*/

potential<size_t>* qgridsystem1D::get_potential() const {
    return m_potential;
}

/*Reference * get_evolver() const {
    return m_evolver;
}*/

#include <iostream>

void qgridsystem1D::_init() {
    std::cerr << "OLEEE" << std::endl;
    // default values
    m_potential = nullptr;
}

void qgridsystem1D::_process(double dt) {
    // default values
    qsim::grid::qsystem1D::evolve(dt);
}
