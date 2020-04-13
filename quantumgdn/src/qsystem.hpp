#pragma once 

#include <Godot.hpp>
#include <Node.hpp>

#include <memory>

namespace godot {
    
    class curve_potential;
    class qsimbox;
    
    class qsystem : public Node {
        GODOT_CLASS(qsystem, Node)
    private:
         qsimbox * _box;
         std::shared_ptr<curve_potential> m_pot;
    public:
         qsystem(std::shared_ptr<curve_potential> ptr = nullptr);
         virtual ~qsystem() = default;

         virtual Vector2 location(size_t) const;

         qsimbox * box() const;

         std::shared_ptr<curve_potential> potential();
         curve_potential * potential_ptr();

         void _init();
         void _ready();

         static void _register_methods();
    };
}
