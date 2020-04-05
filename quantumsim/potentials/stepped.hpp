#pragma once

#include "potential.hpp"

namespace qsim::pot {
       
    template <typename Coords> 
    class stepped : virtual public potential<Coords> {

        double value; 
        std::pair<Coords, Coords> bounds;

    public:

        stepped(double _value, Coords left, Coords right)
            : value(_value), bounds({left, right}) {}
        
        virtual double operator()(const Coords& access) const override {
            return (bounds.first <= access && bounds.second >= access) ? value : 0;
        }
    }; 
}
