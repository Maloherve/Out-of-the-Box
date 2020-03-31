#pragma once

#include <tuple>

namespace qsim::math {
    
    /*
     * General sum composition of a mathematical object
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
        
        // identity multiplication wrapper 
        struct id_t {
            T value;

            id_t(T a) : value(a) {}
            operator T() { return value; }
            
            template<class V>
            V operator<<(V v) const {
                for (auto& elem : v)
                    elem *= value;
                return v; // home it's moved
            }
        };

        /*
         * Chain sum optimizer
         * It allows to move the result until the end
         */
        template<class V>
        struct optimizer {

            V vector; // TODO, check if rvalue or reference

            optimizer(optimizer&& other) : vector(std::move(other.vector)) {}

            optimizer(V&& v) : vector(std::move(v)) {}
           
            // chained sum over the vector 
            optimizer operator+(optimizer&& next) {

                for (size_t k = 0; k < vector.size(); k++)
                    vector[k] += next.vector[k];

                return optimizer(std::move(*this));
            }
            
            // move the final result the a definitive vector by static_cast
            operator V() {
                return std::move(vector);
            }
        };

    public:

        static constexpr size_t N = sizeof...(Obj);
        /*
         * id: first element, multiple of the identity
         * ...c: list of composing elements
         */
        composition(T id, Obj&& ...c) : identity(id), components(std::make_tuple(c...)) {}

        /*
         * Apply a linear operation over all elements
         * Requirements of In:
         *      - T operator[](size_t m) const
         *      - size_t In::size() const
         */
        template<class V>
        V operator*(const V& input) const
        {
            if constexpr (sizeof...(Obj) > 0)
                // apply the operation over the sum of all elements, std=c++17 needed
                return std::apply([&](Obj const& ...objs) -> V { return optimizer<V>(id_t(identity) << input) + (optimizer<V>(objs << input) + ...); }, components);
            else
                return id_t(identity) << input;
        }

        template<class Another>
        qsim::math::composition<T, Obj..., Another> extend(Another&& addon) const {
            return qsim::math::composition<T, Obj..., Another>(identity, (std::get<Obj>(components), ...), addon);
        }
        
        template <typename S>
        qsim::math::composition<T, Obj...>& operator*=(S mult) {
            identity *= mult;
            if constexpr (sizeof...(Obj) > 0)
                std::apply([&mult](Obj& ...obj) { (..., (obj *= mult)); }, components);
            return *this;
        }

        qsim::math::composition<T, Obj...>& operator+=(T add) {
            identity += add;
            return *this;
        }

        qsim::math::composition<T, Obj...>& operator-=(T add) {
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
/*namespace qsim::helper {
    template<class Op, class V>
    struct application {
        V operator()(const Op& op, const V& v) {
            return op * v;
        }
    };
}

template<class ...Obj, class V>
V operator*(const qsim::math::composition<Obj...>& comp, const V& v) {
    return comp.linear(v, qsim::helper::application);
}*/

