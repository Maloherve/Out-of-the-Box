#include "grid.hpp"

#include <RectangleShape2D.hpp>

double qsimbox::x() const {
    return get_position().x;
}

double qsimbox::y() const {
    return get_position().y;
}

void qsimbox::set_width(double l) const {
    shape->shape->extends.x = l/2;
}

double qsimbox::width() const {
    return shape->shape->extends.x * 2;
}

void qsimbox::set_height(double h) const {
    shape->shape->extends.y = h/2;
}

double qsimbox::height() const {
    return shape->shape->extends.y * 2;
}

void qsimbox::_init() {
    shape = new CollisionShape2D();
    add_child(shape);
    shape->shape = RectangleShape2D::_new();
}

void qsimbox::register_methods() {
    register_property<qsimbox, double>("width", &qsimbox::set_width, &qsimbox::width, 1);
    register_property<qsimbox, double>("height", &qsimbox::set_height, &qsimbox::height, 1);
}

/*
 * Grids
 */

size_t grid1D::to_index(double x) {
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
}

