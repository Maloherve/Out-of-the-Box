#include "uniform_field.hpp"
#include <TileMap.hpp>

#include "debug.hpp"
#include <iostream>

using namespace godot;

uniform_field::uniform_field() : value(0.0), tilemap(nullptr) {
}

double uniform_field::at(const Vector2& r) const {
    return (tilemap != nullptr &&
            tilemap->get_cellv(tilemap->world_to_map(r)) != TileMap::INVALID_CELL) 
        ? value : 0.0;
}

double uniform_field::_at(Vector2 r) const {
    return at(r);
}

void uniform_field::_ready() {
    tilemap = Object::cast_to<TileMap>(get_parent());

    if (tilemap == nullptr) {
        std::cout << "WARNING: null tilemap in uniform_field" << std::endl;
        // TODO, send godot warning
    }
}

void uniform_field::_init() {
}

void uniform_field::_register_methods() {
    register_method("at", &uniform_field::_at);
    register_property<uniform_field, double>("value", &uniform_field::value, 0.0);
    register_method("_ready", &uniform_field::_ready);
}
