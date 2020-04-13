#include "curve_field.hpp"

using namespace godot;

double curve_field::at(const Vector2& r) const {
    double Vx = const_cast<Curve*>(*along_x)->interpolate_baked(r.x);
    double Vy = const_cast<Curve*>(*along_y)->interpolate_baked(r.y);
    return Vx + Vy;
}

void curve_field::_init() {
    along_x = Ref<Curve>(Curve::_new());
    along_y = Ref<Curve>(Curve::_new());
}

void curve_field::_register_methods() {
    register_property<curve_field, Ref<Curve>>("V_x", &curve_field::along_x, Curve::_new());
    register_property<curve_field, Ref<Curve>>("V_y", &curve_field::along_y, Curve::_new());
}
