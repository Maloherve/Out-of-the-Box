#pragma once

#include <Godot.hpp>
#include <Resource.hpp>

#include "quantumsim/grid/qsystem1D.hpp"

namespace godot {
    
   /*
    * Base class for all wave initializers
    */ 
    class wave_init1D : public qsim::grid::qsystem1D::init_pack, public Resource {
        GODOT_CLASS(wave_init1D, Resource)
    public:
        wave_init1D(size_t N = 0);
        virtual ~wave_init1D() = default;

        // default is the null function
        virtual qsim::wave_t psi(double x) const {
            return 0;
        }

        void _init();

        static void _register_methods();
    };


   /*
    * Gaussian specialization
    */ 
    class gauss_init1D : public wave_init1D {
        GODOT_SUBCLASS(gauss_init1D, wave_init1D)

            double k0, r0;
            double sigma;

    public:
        gauss_init1D(size_t N = 0,
                     double k0 = 0.0, // space frequency
                     double r0 = 0.0, // peak position
                     double sigma = 1.0); // uncertainty

        virtual qsim::wave_t psi(double x) const override;
        
        void _init();

        void set_sigma(double);
        double get_sigma() const;

        static gauss_init1D * _new(size_t N, 
                                   double k0, 
                                   double r0, 
                                   double sigma);

        static void _register_methods();
    };
}
