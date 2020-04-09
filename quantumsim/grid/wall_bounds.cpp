#include "grid/wall_bounds.hpp"

std::pair<wave_t, wave_t> wall_bounds::continuity() const {
    // free boundary conditions
    return {0,0}
}

wave_t wall_bounds::before()(double) const {
    return 0.0;
}

wave_t wall_bounds::after()(double) const {
    return 0.0;
}

double wall_bounds::normalization() const {
    return 0.0;
}
