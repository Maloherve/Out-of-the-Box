#include "qsimbox.hpp"

#include <CollisionShape2D.hpp>
#include <RectangleShape2D.hpp>
#include "debug.hpp"

using namespace godot;

double qsimbox::x() const {
    return get_position().x;
}

double qsimbox::y() const {
    return get_position().y;
}

void qsimbox::set_width(double l) {
    if (rect != nullptr)
        rect->set_extents(Vector2(l/2, height()/2));
}

double qsimbox::width() const {
    return rect->get_extents().x * 2;
}

void qsimbox::set_height(double h) {
    if (rect != nullptr)
        rect->set_extents(Vector2(width()/2, h/2));
}

double qsimbox::height() const {
    return static_cast<double>(rect->get_extents().y) * 2;
}

void qsimbox::_ready() {

    // loop through children and check for correspondances
    auto list = get_children();
    for (int i = 0; i < list.size(); ++i) {
        Variant child = list[i];
        CollisionShape2D * shp = Object::cast_to<CollisionShape2D>(child);

        if (shp != nullptr) {
            npdebug("A collision shape was found")
            shape = shp;

            RectangleShape2D * rshp = Object::cast_to<RectangleShape2D>(*shape->get_shape());
            if (rshp != nullptr) {
                npdebug("A rectangle shape was found")
                rect = rshp;
                break;
            }
        }
    }
    
    if (rect == nullptr)
        throw std::invalid_argument("qsimbox must have a CollisionShape2D");
}


void qsimbox::_init() {
}

void qsimbox::_register_methods() {
    register_property<qsimbox, double>("width", &qsimbox::set_width, &qsimbox::width, 1.0);
    register_property<qsimbox, double>("height", &qsimbox::set_height, &qsimbox::height, 1.0);
    register_method("_ready", &qsimbox::_ready);
}

