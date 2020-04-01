#pragma once

#include <complex>
#include <memory>
#include "math/composition.hpp"

namespace qsim {
    
    /*
     * Most general description of quantum system
     */
    template <typename Coords, class WaveFunction, class H>
    class qsystem {

        // mass
        double m;

        // the wave function
        WaveFunction wave;

        // a potential, external management
        std::shared_ptr<potential<Coords>> pot,

        // an integrator
        std::shared_ptr<evo::integrator<WaveFunction, H> evolver;
        
    public:
        qsystem(double _m, 
                const WaveFunction& _wave,
                std::shared_ptr<potential<Coords>> _V,
                std::shared_ptr<evo::integrator<WaveFunction, H>> _evolver,
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
        
        // eventually change the behaviour
        virtual void set_mass(double m) {
            this->m = abs(m);
        }

        // evolution in time
        void evolve(double dt) {
            wave = std::move(evolver->evolve(*this, dt));
        }

        inline const WaveFunction& psi() const {
            return wave;
        }

        inline const potential<Coords>& V() const {
            return const_cast<const potential<Coords>&>(*pot);
        }

        // hemiltonian access
        virtual H hemiltonian() const = 0;
        
        // averanges
        virtual double energy() const = 0;
        virtual double position() const = 0;
        virtual double momentum() const = 0;
    };
}
