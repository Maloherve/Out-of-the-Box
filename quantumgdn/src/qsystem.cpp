#include "qsystem.hpp"

#include "qsimbox.hpp"
#include "curve_potential.hpp"

#include "debug.hpp"

using namespace godot;

qsystem::qsystem(std::shared_ptr<curve_potential> pot) : _box(nullptr), m_pot(pot) {}

Vector2 qsystem::location(size_t) const {
    return Vector2();
}

qsimbox * qsystem::box() const {
    return _box;
}

std::shared_ptr<curve_potential> qsystem::potential() {
    return m_pot;
}

curve_potential * qsystem::potential_ptr() {
    return m_pot.get();
}

void qsystem::_init() {}

void qsystem::_ready() {
    qsimbox * parent = Object::cast_to<qsimbox>(get_parent());

    if (parent != nullptr) {
        _box = parent;

        npdebug("Potential pointer: ", m_pot.get())
        //parent->connect("body_entered", m_pot.get(), "_qsimbox_on_body_entered");
        //parent->connect("body_exited", m_pot.get(), "_qsimbox_on_body_exited");
    } else {
        npdebug("The parent is null")
    }
}

void qsystem::_register_methods() {
    npdebug("Registering")
    register_method("box", &qsystem::box);
    register_method("V", &qsystem::potential_ptr);
}

