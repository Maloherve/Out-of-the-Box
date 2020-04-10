#include "grid/qsystem2D.hpp"
#include "constants.hpp"
#include "potential.hpp"

#include "grid/integrators.hpp"

const qsim::math::diagonals<double, 3> qsystem2D::A_x = math::diagonals<double, 3>({math::sdiag_entry(-1, 1.0), math::sdiag_entry(0, -2.0), math::sdiag_entry(1, 1.0)});
const qsim::math::diagonals<double, 3> qsystem2D::A_y = math::diagonals<double, 3>({math::sdiag_entry(-M, 1.0), math::sdiag_entry(0, -2.0), {math::sdiag_entry(M, 1.0));


