#pragma once
#include "boundaries.hpp"

#include "grid/wave.hpp"

namespace qsim::grid {

    class free_bounds : public qsim::interval {
        const wave_vector& wave;
    protected:
        double lower_damping;
        double upper_damping;
    public:
        free_bounds(double low, double up, double low_damp, double up_damp, const wave_vector&);
        // continuity condition
        virtual std::pair<wave_t, wave_t> continuity() const override;

        // wave function value
        virtual wave_t before()(double) const override;
        virtual wave_t after()(double) const override;

        virtual double double normalization() const override;
    }
}
