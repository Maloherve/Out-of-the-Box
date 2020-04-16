#pragma once

#include "quantumsim/potential.hpp"

#include <Godot.hpp>
#include <Object.hpp>

#include <map>
#include <vector>

namespace godot {
    
    class Node; 
    class Node2D;    
    class gdqsystem;
    class potential_field;

    /*
     * A double-buffered potential descriptor
     * Note: buffering is disabled by default
     */
    class curve_potential : public qsim::potential<size_t> {

        gdqsystem * system; 
        std::map<Node2D*, potential_field*> nodes_map;

        typedef double (curve_potential::*behaviour_t)(const size_t&) const;

        behaviour_t behaviour;

        double buffered_access(const size_t&) const;
        double direct_access(const size_t&) const;

        std::vector<double> buffer;

    public:
        curve_potential(gdqsystem * system = nullptr); 
        virtual ~curve_potential() = default;
        
        // interface to grid
        virtual double operator()(const size_t& access) const override;
        
        // access by space coordinates
        double at(const Vector2& v) const; 
        
        // CollisionObject2D auto-detection response 
        void on_body_entered(Node * entry);
        void on_body_exited(Node * entry);

        // buffering initialization
        void freeze(size_t N);
        void set_buffering(bool);
        bool is_buffering() const;
    };
}
