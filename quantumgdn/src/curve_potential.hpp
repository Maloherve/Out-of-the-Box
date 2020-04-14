#pragma once

#include "quantumsim/potential.hpp"

#include <Godot.hpp>
#include <Object.hpp>

#include <map>

namespace godot {
    
    class Node; 
    class Node2D;    
    class gdqsystem;
    class potential_field;

    class curve_potential : public qsim::potential<size_t> {

        gdqsystem * system; 
        std::map<Node2D*, potential_field*> nodes_map;

    public:
        curve_potential(gdqsystem * system = nullptr); 
        virtual ~curve_potential() = default;
        
        // interface to grid
        virtual double operator()(const size_t& access) const override;

        double at(const Vector2& v) const; 
        
        void on_body_entered(Node * entry);
        void on_body_exited(Node * entry);
    };
}
