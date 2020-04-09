#pragma once

#include "boundaries.hpp"

namespace qsim::grid {

    class wall_bounds : public qsim::interval {
    public:
        using qsim::interval::interval;

        // continuity condition
        virtual std::pair<wave_t, wave_t> continuity() const override;

        // wave function value
        virtual wave_t before()(double) const override;
        virtual wave_t after()(double) const override;

        virtual double double normalization() const override;
    }
}
