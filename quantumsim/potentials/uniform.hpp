#pragma once

#include "potential.hpp"

namespace qsim::pot {

    template <typename Coords> 
    class uniform : public potential<Coords> {

        double value; 

    public:

        uniform(double _value = 0) noexcept : value(_value) {}
        
        virtual double operator()(const Coords&) const override {
            return value;
        }
    };
}
