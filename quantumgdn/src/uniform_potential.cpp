#include "uniform_potential.hpp"

using namespace godot;


uniform_potential::uniform_potential() {
    // set the superclass pointer
    POTENTIAL_CLASS(this, size_t)
}

uniform_potential::~uniform_potential() {}

void uniform_potential::_init() {
    // nothing to do
}

void uniform_potential::_register_methods() {
    // direct methods
    register_method("get", &uniform_potential::access);
    //register_method("at", &uniform::);
}

double uniform_potential::access(size_t m) {
    return (*this)(m);
}
