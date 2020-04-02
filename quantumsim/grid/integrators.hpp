#pragma once

namespace qsim::grid {
    
    template<class ... Coords>
    struct neighbourhood {

        static_assert((std::is_integral<Coords>::value && ...));

        const grid_wave& vector;
        std::tuple<Coords ...> jumps;

        grid_neighbourhood(const grid_wave& _vector, Coords&& ... args)
            : vector(_vector), jumps(args) {}

        const wave_t& at(size_t offset, size_t k, const Coords& ... cs) const {
            return std::apply([&](const Coords& ... Ns) {vector[offset + k + ((Ns * cs) + ...) ]}, jumps);
        }
    };
    
    // simplest integrator ever 
    template<class Op, class ... Jumps>    
    wave_t grid_integrate(const grid_wave& v, const Op& op, double dV, const Jumps ... jumps) {
        wave_t out(0);

        for (size_t k = 0; k < v.size(); ++k)
            out += v[k] * ;

        return out *= dV;
    }
}
