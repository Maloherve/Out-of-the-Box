#include "curve_potential.hpp"

#include <Node2D.hpp>
#include <TileMap.hpp>

#include "potential_field.hpp"
#include "gdqsystem.hpp"

#include "debug.hpp"

using namespace godot;

curve_potential::curve_potential(gdqsystem * _system) 
    : system(_system), behaviour(&curve_potential::direct_access)
{
}

double curve_potential::buffered_access(const size_t& index) const {
    return buffer[index]; 
}

double curve_potential::direct_access(const size_t& index) const {
    return at(system->location(index));
}

// interface to grid
double curve_potential::operator()(const size_t& access) const  {
    return (this->*behaviour)(access);
}

#include <iostream>

double curve_potential::at(const Vector2& v) const  {
    double out(0);
    for (const auto& entry : nodes_map) {
        // get location relatively to the system box
        Transform2D B = entry.first->get_global_transform();
        Transform2D A = system->get_global_transform();
        // then get relative position to the object
        // don't know if definitive, vertical mode?
        Vector2 r = B.xform_inv(A.xform(v)) / system->get_global_scale();
        out += entry.second->at(r);
    }

    return out;
}

void curve_potential::on_body_entered(Node * entry) {
    potential_field * wrap = nullptr;

    // look through children the first which matches the condition
    // to a be a potential_field
    auto list = entry->get_children();
    for (int i = 0; i < list.size() && (wrap = Object::cast_to<potential_field>(list[i])) == nullptr; ++i) {}

    if (wrap == nullptr) {
        std::cout << "No potential_field child detected in: " << entry << std::endl;
        return; // none found
    }

    Node2D * key; 
    if (key = Object::cast_to<Node2D>(entry)) {
        std::cout << "Adding object: " <<  key << std::endl;
        nodes_map[key] = wrap;
    }
}

void curve_potential::on_body_exited(Node * entry) {
    npdebug("OK exited")
    Node2D * key;
    if (key = Object::cast_to<Node2D>(entry))
        nodes_map.erase(key);
}

void curve_potential::freeze(size_t N) {

    for (size_t k = 0; k < buffer.size(); ++k)
        buffer[k] = direct_access(k); // deduce values in k

    buffer.reserve(N);
    
    size_t i(N);
    while (buffer.size() < N)
        buffer.push_back(direct_access(i++));
}

void curve_potential::set_buffering(bool enable) {
        behaviour = (enable) ? 
            &curve_potential::buffered_access :
            &curve_potential::direct_access;
}

bool curve_potential::is_buffering() const {
    return behaviour == &curve_potential::buffered_access;
}

