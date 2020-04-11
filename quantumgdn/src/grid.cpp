#include "grid.hpp"

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
    rect->set_extents(Vector2(l/2, height()));
}

double qsimbox::width() const {
    return rect->get_extents().x * 2;
}

void qsimbox::set_height(double h) {
    rect->set_extents(Vector2(width(), h/2));
}

double qsimbox::height() const {
    return rect->get_extents().y * 2;
}

void qsimbox::_init() {
    npdebug("Initialize")
    shape = new CollisionShape2D();
    add_child(shape);
    rect = RectangleShape2D::_new();
    shape->set_shape(rect);
    npdebug("End initialize")
}

void qsimbox::_register_methods() {
    register_property<qsimbox, double>("width", &qsimbox::set_width, &qsimbox::width, 1.0);
    register_property<qsimbox, double>("height", &qsimbox::set_height, &qsimbox::height, 1.0);
}

/*
 * Grids
 */

/*size_t grid1D::to_index(double x) {
    return x - get_position().x
}

double grid1D::to_space(size_t i) {
}

size_t grid2D::to_index(double x, double x) {
}

size_t grid2D::to_index(const Vector2& v) {
}

Vector2 grid2D::to_space(size_t k) {
}

Vector2 grid2D::to_space(size_t i, size_t j) {
}*/

