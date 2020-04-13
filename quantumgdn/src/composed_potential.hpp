#pragma once

#include "quantumsim/potentials/composed.hpp"
#include "potential.hpp"
#include "qsimbox.hpp"

#include <PhysicsBody2D.hpp>
#include <TileMap.hpp>

#include "potential_body.hpp"

#include <map>

namespace godot {
    
    template<class Coords, class Degrees>
    class composed_potential : public potential_obj<Coords, qsim::pot::composed<Coords>, Degrees, double> {
        typedef potential_obj<Coords, qsim::pot::composed<Coords>, Degrees, double> Base;
        GODOT_SUBCLASS(composed_potential, Base)
    
    protected:
        std::map<PhysicsBody2D*, potential<Coords, Degrees>*> body_map;
        std::map<TileMap*, potential<Coords, Degrees>*> tile_map;

    public:
        composed_potential() {}
        ~composed_potential() {}

        /*
         * Godot setup
         */

        void _init() {
        }

        void _ready() {
            Base::_ready();
            if (system != nullptr) {
                qsimbox * bounds = system->box();
                // connect to the boundary area
                composed_potential::connect("body_entered", bounds, "on_body_entered");
                composed_potential::connect("body_exited", bounds, "on_body_exited");
            }
        }

        void on_body_entered(Node * entry) {
            
            potential<Coords, Degrees> * wrap = nullptr;

            // look through children the first which matches the condition
            // to a be a potential_body
            auto list = entry->get_children();
            for (int i = 0; i < list.size(); ++i) {
                Variant child = list[i];
                wrap = Object::cast_to<potential<Coords, Degrees>>(child);
            }

            if (wrap == nullptr)
                return; // none found
            
            // get effective potential grid object
            push_back(*wrap);

            PhysicsBody2D * body; 
            TileMap * map; 
            if (body = Object::cast_to<PhysicsBody2D>(entry)) {
                body_map[body] = wrap;
            } else if (map = Object::cast_to<TileMap>(entry)) {
                tile_map[map] = wrap;
            }
        }

        void on_body_exited(Node * entry) {
            PhysicsBody2D * body; 
            TileMap * map; 

            if (body = Object::cast_to<PhysicsBody2D>(entry)) {
                auto wrap = body_map.find(body);
                if (wrap != body_map.end())
                    remove(*wrap.second);
                body_map.remove(wrap);
            } else if (map = Object::cast_to<TileMap>(entry)) {
                tile_map[map] = wrap;
                auto wrap = tile_map.find(map);
                if (wrap != body_map.end())
                    remove(*wrap.second);
                tile_map.remove(wrap);
            }
        }

        static void _register_methods() {
            register_method("on_body_entered", &composed_potential::on_body_entered);
            register_method("on_body_exited", &composed_potential::on_body_exited);
        }
    };
    
    /*
     * Specializations
     */
    // 1D grid
    class grid_composed_potential1D : public composed_potential<size_t, double> {
        typedef composed_potential<size_t, double> base;
        GODOT_SUBCLASS(grid_composed_potential1D, base)
    public:
        using composed_potential<size_t, double>::composed_potential;
        using composed_potential<size_t, double>::_init;

        static grid_composed_potential1D * _new(double value) {
            auto *ptr = grid_composed_potential1D::_new();
            ptr->_set_value(value);
            return ptr;
        }

        virtual double at(const double& x) const override {

        }

        static void _register_methods() {}
    };

    // 2D grid
    class grid_composed_potential2D : public composed_potential<size_t, Vector2> {
        typedef composed_potential<size_t, double> base;
        GODOT_SUBCLASS(grid_composed_potential2D, base)
    public:
        using composed_potential<size_t, Vector2>::composed_potential;
        using composed_potential<size_t, Vector2>::_init;

        static grid_composed_potential2D* _new(double value) {
            auto *ptr = grid_composed_potential2D::_new();
            ptr->_set_value(value);
            return ptr;
        }


        virtual double at(const Vector2&) const override {

        }

        static void _register_methods() {}
    };
}
