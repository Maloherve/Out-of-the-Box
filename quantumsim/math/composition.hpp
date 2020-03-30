#pragma once

#include <tuple>

namespace qsim::math {
    
    /*
     * General composition of an object
     * This structure take advantages where properties
     * applies to all components singularly (ex: linear operations)
     */
    template<class ...Obj>
    struct composition {

        composition(Obj&& ...c) : components(std::make_tuple(c...)) {}

        std::tuple<Obj...> components;

        template<class Out, class In, class Op>
        Out linear(const In& input, Op&& A) const
        {
            // apply the operation over the sum of all elements
            return (A(std::get<Obj>(components), input) + ...);
        }
    };
}

/*
 * Push element to a composition
 */
template<class ...Obj, class Another>
qsim::math::composition<Obj..., Another>&& operator+(qsim::math::composition<Obj...>&& input, Another&& addon) {
    // expand to tuple and push the new element
    return out((std::get<Obj>(input.components) ...), addon);
}

/*
 * Element-by-element multiplication
 */
template<class ...Obj, class V>
V operator*(const qsim::math::composition<Obj...>& comp, const V& v) {
    return comp.linear(v, [&](auto&& obj, const V& a) -> { return obj * a; });
}

