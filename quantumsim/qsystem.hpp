#pragma once

#include "potential.hpp"
#include <complex>
#include <memory>
#include <cmath>

namespace qsim {

    template <typename Coords>
    class qsystem {

        // a potential
        std::shared_ptr<potential<Coords>> pot;

        // mass
        double m;
        
    public:
        qsystem(double _m, std::shared_ptr<potential<Coords>> _V)
            : pot(_V), m(_m) {

            if (pot == nullptr) {
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
        virtual void evolve(double dt) = 0;

        // access to wave function
        virtual std::complex psi(Coords) const = 0;
        
        // averanges
        virtual double energy() const = 0;
        virtual double position() const = 0;
        virtual double momentum() const = 0;
    };
}
