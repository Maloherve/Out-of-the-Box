#include <Reference.hpp>

#include "quantumsim/grid/qsystem2D.hpp"

namespace godot {
    
   /*
    * Base class for all wave initializers
    */ 
    class wave_init2D : public qsim::grid::qsystem2D::init_pack, public Reference {
        GODOT_CLASS(wave_init2D, Reference)
    public:
        using qsim::grid::qsystem2D::init_pack::init_pack;
        ~wave_init2D() = default;

        qsim::wave_t operator*(double x, double y) const;

        void _init();

        static void _register_methods();
    };


   /*
    * Gaussian specialization
    */ 
    class gauss_init2D : public wave_init2D {
        GODOT_SUBCLASS(gauss_init2D, wave_init2D)

    public:
        gauss_init2D(size_t N = 0,
                     size_t M = 0,
                     Vector2 k0 = Vector2(0.0, 0.0), // space frequency
                     Vector2 r0 = Vector2(0.0, 0.0), // peak position
                     Vector2 sigma = Vector2(1.0, 1.0)); // uncertainty
        
        // TODO, _new
        void _init() {}

        static void _register_methods() {}
    };
}
