#pragma once

#include "fwd.hpp"
#include "qsystem.hpp"
#include "math/composition.hpp"
#include "math/diagonals.hpp"
#include "math/diagonal.hpp"

#include "math/matrix.hpp"

namespace qsim::grid {

    typedef qsim::math::composition<
                              wave_t, 
                              math::diagonals<wave_t, 3>, //dx
                             > laplace_t;


    // concretization for a 2D grid
    class qsystem2D : public qsystem<std::pair<size_t,size_t>, math::matrix<size_t>> {
    private:

        static const math::diagonals<wave_t, 3> A;
 
        double dx; // discretization along x
        double dy; // discretization along y

        // determine first hamiltonian term as function of the mass and the discretization step
        qsim::math::diagonals<wave_t, 3> H_zero_x() const;
        qsim::math::diagonals<wave_t, 3> H_zero_y() const;

    public: 

        struct init_pack {
            std::function<qsim::wave_t (double, double)> f;
            size_t N;
            size_t M;

            init_pack(const std::function<qsim::wave_t (double, double)>& _f = std::function<qsim::wave_t (double, double)>(),
                      size_t _N = 0,
                      size_t _M = 0
                     ) : f(_f), N(_N), M(_M) {}

            inline qsim::wave_t operator()(double x, double y) const {
                return f(x,y);
            }

            wave_vector generate(double dx, double dy) const;
        };

        qsystem2D(double _m, 
                  double _dx, double _dy,
                  std::shared_ptr<potential<size_t>> _V,
                  const init_pack& init = init_pack(),
                  std::shared_ptr<evolver> _evolver = nullptr,
                  double hbar = 1.0
                  );
        
        virtual void evolve(double) override;
        
        // integrals
        virtual double norm() const override;
        virtual double energy() const override;
        std::pair<double,double> position() const;
        std::pair<double,double> momentum() const;
        
        // mapping
        inline double x(size_t i) const {
            return static_cast<double>(i) * dx;
        }

        inline double y(size_t j) const {
            return static_cast<double>(j) * dy;
        }

        /*
         * Causing of boundaries size is decreased of 2
         */
        
        size_t N() const;
        size_t M() const;

        /*
         * iterator tool
         */

        class iterator {
            qsystem2D& sys;
            size_t i, j; // i = 1,...,N ; j = 1,...,M

            void increment();

        public:

            iterator(qsystem2D&, size_t i, size_t j);

            iterator& operator++() {
                increment();                  
                return *this;
            }

            iterator operator++(int) {
                auto it = iterator(*this);
                increment();
                return it;
            }

            bool operator!=(const iterator&) const;
            qsim::wave_t& operator*();

            qsim::wave_t& up();
            qsim::wave_t& down();
            qsim::wave_t& left();
            qsim::wave_t& right();

            double x() const;
            double y() const;
        };

        class const_iterator {
            const qsystem2D& sys;
            size_t i, j; // i = 1,...,N ; j = 1,...,M

            void increment();

        public:
            const_iterator(const qsystem2D&, size_t i, size_t j);

            const_iterator& operator++() {
                increment();                  
                return *this;
            }

            const_iterator operator++(int) {
                auto it = const_iterator(*this);
                increment();
                return it;
            }

            bool operator!=(const const_iterator&) const;
            const qsim::wave_t& operator*() const;

            const qsim::wave_t& up() const;
            const qsim::wave_t& down() const;
            const qsim::wave_t& left() const;
            const qsim::wave_t& right() const;

            double x() const;
            double y() const;
        };

        /*
         * iterators accessors
         */

        iterator begin();
        const_iterator begin() const;

        iterator end();
        const_iterator end() const;
    };
}
