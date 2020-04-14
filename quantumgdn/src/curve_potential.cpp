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
        Transform2D B = entry.first->get_global_transform();
        Transform2D A = system->get_global_transform();
        //npdebug("Request position: (", v.x, ", ", v.y, ")")
        // transform v to global
        // then get relative position to the object
        //Vector2 g = ;
        //npdebug("Global position: (", g.x, ", ", g.y, ")")
        Vector2 r = B.xform_inv(A.xform(v + system->get_size() / 2.0));
        //npdebug("Transformed position: (", r.x, ", ", r.y, ")")
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
        npdebug("No potential_field child detected in: ", entry)
        return; // none found
    }

    Node2D * key; 
    if (key = Object::cast_to<Node2D>(entry)) {
        npdebug("Adding object: ", key)
        nodes_map[key] = wrap;
    }
}

void curve_potential::on_body_exited(Node * entry) {
    npdebug("OK exited")
    Node2D * key;
    if (key = Object::cast_to<Node2D>(entry))
        nodes_map.erase(key);
}

