#include "gdqsystem.hpp"

#include "Area2D.hpp"
#include "curve_potential.hpp"

#include "debug.hpp"

using namespace godot;

gdqsystem::gdqsystem(std::shared_ptr<curve_potential> pot) : m_pot(pot) {}

Vector2 gdqsystem::location(size_t) const {
    return Vector2();
}

std::shared_ptr<curve_potential> gdqsystem::potential() const {
    return m_pot;
}

void gdqsystem::_init() {}

void gdqsystem::_ready() {
    Area2D * parent = Object::cast_to<Area2D>(get_parent());

    if (parent != nullptr) {
        npdebug("Connecting signals")
        parent->connect("body_entered", this, "_qsimbox_on_body_entered");
        parent->connect("body_exited", this, "_qsimbox_on_body_exited");
    } else {
        npdebug("The parent is undefined")
    }
}

void gdqsystem::set_width(double w) {
    if (w > 0)
        width = w;
}

double gdqsystem::get_width() const {
    return width;
}

void gdqsystem::set_height(double h) {
    if (h > 0)
        height = h;
}

double gdqsystem::get_height() const {
    return height;
}

void gdqsystem::on_body_entered(Node * entry) {
    return m_pot->on_body_entered(entry);
}

void gdqsystem::on_body_exited(Node * entry) {
    return m_pot->on_body_exited(entry);
}

void gdqsystem::_register_methods() {

    register_property<gdqsystem, double>("width", &gdqsystem::set_width, &gdqsystem::get_width, 20);
    register_property<gdqsystem, double>("height", &gdqsystem::set_height, &gdqsystem::get_height, 20);

    register_method("_qsimbox_on_body_entered", &gdqsystem::on_body_entered);
    register_method("_qsimbox_on_body_exited", &gdqsystem::on_body_exited);
}

