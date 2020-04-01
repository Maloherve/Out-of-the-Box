#pragma once

namespace qsim {
    
    template<size_t N>
    struct grid_neighbourhood {
        const grid_wave& vector;
        size_t offset;
        std::array<size_t, N-1> jumps;

        grid_neighbourhood(const grid_wave& _vector, size_t _offset, std::array<size_t, N-1> args)
            : vector(_vector), offset(_offset), jumps(args) {}

        const wave_t& at(Ns ... coords) const {
            return vector[offset + k]
        }
    };
    
    // simplest integrator ever 
    template<class Op>    
    wave_t grid_integrate(const grid_wave& v, const Op& op, double dV) {
        wave_t out(0);

        for (size_t k = 0; k < v.size(); ++k)
            out += v[k];

        return out *= dV;
    }
}
