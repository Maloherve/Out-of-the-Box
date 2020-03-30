#pragma once

#include <tuple>

namespace qsim::math {
    
    /*
     * General composition of an object
     * This structure take advantages where properties
     * applies to all components singularly (ex: linear operations)
     *
     * T defines the field over the matrix is defined (integral, real, complex, ecc..)
     */
    template<typename T, class ...Obj>
    class composition {

        // identity (type safety)
        T identity; 
        
        // storage tuple
        std::tuple<Obj...> components;

    public:
        /*
         * id: first element, multiple of the identity
         * ...c: list of composing elements
         */
        composition(T id, Obj&& ...c) : identity(id), components(std::make_tuple(c...)) {}

        // apply a linear operation over all elements
        template<class Out, class In, class Op>
        Out linear(const In& input, Op&& A) const
        {
            // apply the operation over the sum of all elements, std=c++17 needed
            return std::apply([&](Obj const& ...objs) { return A(identity, input) + (A(objs, input) + ...); }, components);
        }

        template<class Another>
        qsim::math::composition<T, Obj..., Another> extend(Another&& addon) const {
            return qsim::math::composition<T, Obj..., Another>(identity, (std::get<Obj>(components), ...), addon);
        }

        qsim::math::composition<T, Obj..., Another>& operator+=(T add) {
            identity += add;
            return *this;
        }

        qsim::math::composition<T, Obj..., Another>& operator-=(T add) {
            identity -= add;
            return *this;
        }
    };
}

/*
 * double and complex addiction (specialization)
 */
template<typename T, class ...Obj, typename std::enable_if<std::is_floating_point<T>::value>::type>
qsim::math::composition<T, Obj...> operator+(qsim::math::composition<T, Obj...> input, T add) {
    // add to the first element
    return input += add;
}

template<typename T, class ...Obj, typename std::enable_if<std::is_floating_point<T>::value>::type>
qsim::math::composition<T, Obj...> operator+(T add, qsim::math::composition<T, Obj...> input) {
    // add to the first element
    return input += add;
}

template<typename T, class ...Obj, typename std::enable_if<std::is_floating_point<T>::value>::type>
qsim::math::composition<T, Obj...> operator-(qsim::math::composition<T, Obj...> input, T add) {
    // add to the first element
    return input -= add;
}

template<typename T, class ...Obj, typename std::enable_if<std::is_floating_point<T>::value>::type>
qsim::math::composition<T, Obj...> operator-(T add, qsim::math::composition<T, Obj...> input) {
    // add to the first element
    return input -= add;
}

/*
 * Push element into a new composition if Another is not a floating point
 */
template<typename T, class ...Obj, class Another, class Enable = void>
qsim::math::composition<T, Obj..., Another> operator+(qsim::math::composition<T, Obj...>&& input, Another&& addon) {
    // expand to tuple and push the new element
    return qsim::math::composition<T, Obj..., Another>(input.identity, (std::get<Obj>(input.components), ...), addon);
}

/*
 * Element-wise multiplication
 */
template<class ...Obj, class V>
V operator*(const qsim::math::composition<Obj...>& comp, const V& v) {
    return comp.linear(v, [&](auto&& obj, const V& a) -> { return obj * a; });
}

