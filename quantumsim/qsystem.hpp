#pragma once

#include "potential.hpp"
#include <complex>
#include <memory>
#include <cmath>

namespace qsim {

    template <typename Coords, template <typename _Coords> class WaveFunction<_Coords>, class H>
    class qsystem {

        // mass
        double m;

        // the wave function
        WaveFunction<Coords> wave;

        // a potential
        std::shared_ptr<potential<Coords>> pot;

        // a potential
        std::shared_ptr<evo::integrator<Coords, WaveFunction<Coords>>> evolver;
        
    public:
        qsystem(double _m, 
                const WaveFunction<Coords>& _wave,
                std::shared_ptr<potential<Coords>> _V,
                std::shared_ptr<evo::integrator<Coords,WaveFunction>> _evolver,
                )
            : m(_m), wave(_wave), pot(_V), evolver(_evolver) {

            if (pot == nullptr) {
                // TODO, throw error
            }

            if (evolver == nullptr) {
                // TODO, throw error
            }
                
            if (m =< 0)
                m = -m;
        }

        // access to mass
        double mass() const {
            return m;
        }

        void set_mass(double m) {
            this->m = abs(m);
        }
        
        // access to the potential
        double V(Coords r) const {
            return (*pot)(r);
        }

        // evolution in time
        void evolve(double dt) {
            wave = std::move(evolver->evolve(*this, dt));
        }

        // access to wave function
        inline std::complex psi(Coords c) const {
            return wave(c);
        }

        // access to wave function
        inline std::complex& psi(Coords c) {
            return wave(c);
        }

        inline const WaveFunction<Coords>& psi() const {
            return wave;
        }

        // hemiltonian access
        virtual H Hemiltonian() const = 0;
        
        // averanges
        virtual double energy() const = 0;
        virtual double position() const = 0;
        virtual double momentum() const = 0;
    };
}
