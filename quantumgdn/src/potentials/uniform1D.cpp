#include "uniform.hpp"

using namespace qgdn;

void uniform::_register_methods() {
    // direct methods
    register_method("get", &uniform::operator());
    register_method("at", &uniform::);
}
