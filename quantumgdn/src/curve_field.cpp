#include "curve_field.hpp"

#include "debug.hpp"

using namespace godot;

double curve_field::at(const Vector2& r) const {
    double Vx = const_cast<Curve*>(*along_x)->interpolate_baked((r.x - get_position().x) / (2.0 * get_scale().x) + 0.5);
    double Vy = const_cast<Curve*>(*along_y)->interpolate_baked((r.y - get_position().y) / (2.0 * get_scale().y) + 0.5);
    return Vx + Vy;
}

double curve_field::_at(Vector2 r) const {
    return at(r);
}

void curve_field::_init() {
    along_x = Ref<Curve>(Curve::_new());
    along_y = Ref<Curve>(Curve::_new());
}

void curve_field::_register_methods() {
    register_method("at", &curve_field::_at);
    register_property<curve_field, Ref<Curve>>("V_x", &curve_field::along_x, Curve::_new());
    register_property<curve_field, Ref<Curve>>("V_y", &curve_field::along_y, Curve::_new());
}
