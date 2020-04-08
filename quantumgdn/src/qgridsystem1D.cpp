#include "qgridsystem1D.hpp"

#include "quantumsim/evolvers/explicit.hpp"
#include "wave_packets.hpp"
#include "uniform_potential.hpp"

#include "debug.hpp"

using namespace godot;
using namespace qsim::grid;
using namespace qsim::pot;


qbounds1D::qbounds1D(qsim::grid::qsystem1D::bound * _ptr)
    : ptr(_ptr) {}

void qbounds1D::set_pos(double _pos) {
    ptr->location = _pos;
}

double qbounds1D::get_pos() const {
    return ptr->location;
}

void qbounds1D::set_mode(int _mode) {
    ptr->mode = static_cast<qsim::boundary_mode>(_mode);
}

int qbounds1D::get_mode() const {
    return static_cast<int>(ptr->mode);
}

void qbounds1D::_register_methods() {
    register_property<qbounds1D, double>("location", &qbounds1D::set_pos, &qbounds1D::get_pos, 0.0);
    register_property<qbounds1D, int>("mode", &qbounds1D::set_mode, &qbounds1D::get_mode, int(qsim::boundary_mode::free));
}

qgridsystem1D::qgridsystem1D() 
    : qsystem1D(1.0, 
                {-1.0, 1.0},
                wave_vector(1, 0.0), // trivial wave function
                std::make_shared<qsim::pot::uniform<size_t>>(), // zero potential, not a godot reference
                std::make_shared<qsystem1D::explicit_evolver>()),
    wave_fct(nullptr), m_potential(nullptr) {}

void qgridsystem1D::_init() {
    wave_fct = grid_wave::_new();
    wave_fct->set_instance(&psi());
    add_child(wave_fct);
}

qgridsystem1D::~qgridsystem1D() {
    npdebug("Freeing system")
}

void qgridsystem1D::_register_methods() {
    // godot built-in
    register_method("_physics_process", &qgridsystem1D::_fixed_process);
    register_method("_ready", &qgridsystem1D::_ready);

    // direct methods
    register_method("energy", &qgridsystem1D::_energy);
    register_method("position", &qgridsystem1D::_position);
    register_method("momentum", &qgridsystem1D::_momentum);
    register_method("normalize", &qgridsystem1D::_normalize);
    register_method("map", &qgridsystem1D::_map);
    register_method("size", &qgridsystem1D::_size);
    
    // properties
    register_property<qgridsystem1D, double>("mass", &qgridsystem1D::set_mass, &qgridsystem1D::mass, 1.0);
    register_property<qgridsystem1D, Ref<qbounds1D>>("upper_bound", &qgridsystem1D::_set_upper_bound, &qgridsystem1D::_get_upper_bound, nullptr);
    register_property<qgridsystem1D, Ref<qbounds1D>>("lower_bound", &qgridsystem1D::_set_lower_bound, &qgridsystem1D::_get_lower_bound, nullptr);
    register_property<qgridsystem1D, grid_potential*>("V", &qgridsystem1D::_set_potential, &qgridsystem1D::_get_potential, nullptr);
    register_property<qgridsystem1D, grid_wave*>("psi", &qgridsystem1D::_set_wave, &qgridsystem1D::_get_wave, nullptr);
}


double qgridsystem1D::_energy() const {
    return energy();
}

double qgridsystem1D::_position() const {
    return position();
}

double qgridsystem1D::_momentum() const {
    return momentum();
}

double qgridsystem1D::_normalize() {
    return normalize();
}

double qgridsystem1D::_map(size_t m) const {
    return map(m);
}

double qgridsystem1D::_size() const {
    return size();
}

void qgridsystem1D::_set_potential(grid_potential * pot) {

    if (!(pot != nullptr && pot->is_safe())) {
        npdebug("Aptempting to initialize a null pointer or an unsafe reference")
        npdebug("Godot reference grid potential pointer: ", pot)
        // TODO throw godot exception
        return;
    }

    m_potential = pot;
    
    // finally set the potential 
    qsim::grid::qsystem1D::set_potential(*pot); 
}

grid_potential * qgridsystem1D::_get_potential() const {
    return m_potential;
}


void qgridsystem1D::_set_wave(grid_wave *manager) {

    if (manager != nullptr)
        manager->set_instance(&psi());

    wave_fct = manager; 
}

grid_wave * qgridsystem1D::_get_wave() const {
    return wave_fct;
}

void qgridsystem1D::_set_upper_bound(Ref<qbounds1D> up) {
    if (up != nullptr)
        up->set_instance(&upper_bound());
    up_bound = up;
}

void qgridsystem1D::_set_lower_bound(Ref<qbounds1D> low) {
    if (low != nullptr)
        low->set_instance(&lower_bound());
    low_bound = low;
}

Ref<qbounds1D> qgridsystem1D::_get_upper_bound() const {
    return up_bound;
}

Ref<qbounds1D> qgridsystem1D::_get_lower_bound() const {
    return low_bound;
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


/*Reference * get_evolver() const {
    return m_evolver;
}*/

void qgridsystem1D::_ready() {
    npdebug("Normalizing function")
    qsim::grid::qsystem1D::normalize();
}

void qgridsystem1D::_fixed_process(double dt) {
    // default values
    npdebug("Evolving system")
    qsim::grid::qsystem1D::evolve(dt);
    qsim::grid::qsystem1D::normalize();
}
