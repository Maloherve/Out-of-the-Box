#pragma once
#include <Reference.hpp>

#include "quantumsim/grid/qsystem2D.hpp"

namespace godot {
    
   /*
    * Base class for all wave initializers
    */ 
    class wave_init2D : public qsim::grid::qsystem2D::init_pack, public Reference {
        GODOT_CLASS(wave_init2D, Reference)
    public:
        wave_init2D(size_t N = 0, size_t M = 0);
        ~wave_init2D() = default;

        // default is the null function
        virtual qsim::wave_t psi(double x, double y) const {
            return 0;
        }

        void _init();

        static void _register_methods();
    };


   /*
    * Gaussian specialization
    */ 
    class gauss_init2D : public wave_init2D {
        GODOT_SUBCLASS(gauss_init2D, wave_init2D)

            Vector2 k0, r0;
            Transform2D sigma;

    public:
        gauss_init2D(size_t N = 0,
                     size_t M = 0,
                     Vector2 k0 = Vector2(0.0, 0.0), // space frequency
                     Vector2 r0 = Vector2(0.0, 0.0), // peak position
                     Transform2D sigma = Transform2D(1.0, 0.0, 0.0, 1.0, 0.0, 0.0)); // uncertainty

        virtual qsim::wave_t psi(double x, double y) const override;
        
        void _init();

        void set_sigma(const Transform2D&);
        Transform2D get_sigma() const;

        static gauss_init2D * _new(size_t N, 
                                   size_t M, 
                                   Vector2 k0, 
                                   Vector2 r0, 
                                   Transform2D sigma);

        static void _register_methods();
    };
}
