#include "curve_potential.hpp"

#include <Node2D.hpp>
#include <TileMap.hpp>

#include "potential_field.hpp"
#include "gdqsystem.hpp"

#include "debug.hpp"

using namespace godot;

curve_potential::curve_potential(gdqsystem * _system) 
    : system(_system)
{
}

// interface to grid
double curve_potential::operator()(const size_t& access) const  {
    return at(system->location(access));
}

 double curve_potential::at(const Vector2& v) const  {
    double out(0);

    for (const auto& entry : nodes_map) {
        // get location relatively to the system box
        Transform2D T = entry.first->get_relative_transform_to_parent(system);
        out += entry.second->at(T.xform(v));
    }

    return out;
}

void curve_potential::on_body_entered(Node * entry) {
    npdebug("OK entered")
    potential_field * wrap = nullptr;

    // look through children the first which matches the condition
    // to a be a potential_field
    auto list = entry->get_children();
    for (int i = 0; i < list.size(); ++i) {
        Variant child = list[i];
        wrap = Object::cast_to<potential_field>(child);
    }

    if (wrap == nullptr)
        return; // none found

    Node2D * key; 
    if (key = Object::cast_to<Node2D>(entry))
        nodes_map[key] = wrap;
}

void curve_potential::on_body_exited(Node * entry) {
    npdebug("OK exited")
    Node2D * key;
    if (key = Object::cast_to<Node2D>(entry))
        nodes_map.erase(key);
}

