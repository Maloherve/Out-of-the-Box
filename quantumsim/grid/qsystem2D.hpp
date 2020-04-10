#pragma once

#include "grid/qgridsystem.hpp"
#include "math/composition.hpp"
#include "math/diagonals.hpp"
#include "math/diagonal.hpp"
#include "evolvers/evofw.hpp"

namespace qsim::grid {

    typedef qsim::math::composition<
                              double, 
                              math::diagonals<double, 3>, //dx
                              math::diagonals<double, 3>, //dy
                              math::diag_functor<double>
                             > H_matrix_2D;

    typedef grid_H<H_matrix_2D> grid_H_2D;
    

    // concretization for a 2D grid
    class qsystem2D : public qgridsystem<H_matrix_2D> {
    private:

        // hamiltonian object
        H_matrix_2D H; // non-constant, the mass could change
        
        // standard matrix A, y is variable
        static const math::diagonals<double, 3> A_x;
        static const math::diagonals<double, 3> A_y(size_t M);

        double dx; // discretization along x
        double dy; // discretization along y

        size_t _N; // boundary limit
        size_t _M; // boundary limit
        
        // determine first hamiltonian term in function of the mass and the discretization step
        qsim::math::diagonals<double, 3> H_zero_x() const;
        qsim::math::diagonals<double, 3> H_zero_y() const;

        inline size_t map(size_t i, size_t j) const {
            return (_M - 1) * i + j;
        }

    public: 

        struct init_pack {
            std::function<qsim::wave_t (double, double)> f;
            size_t N;
            size_t M;

            init_pack(const std::function<qsim::wave_t (double, double)>& _f = std::function<qsim::wave_t (double, double)>(),
                      size_t _N = 0,
                      size_t _M = 0
                     ) : f(_f), N(_N), M(_M) {}

            wave_vector generate(double dx, double dy) const;
        };

        // possible integrators' forward declaration
        typedef evo::explicit_scheme<size_t, wave_vector, grid_H_2D> explicit_evolver;

        qsystem2D(double _m, 
                  double _dx, double _dy,
                  std::shared_ptr<potential<size_t>> _V,
                  const init_pack& init = init_pack(),
                  std::shared_ptr<evolver<size_t, wave_vector, grid_H_2D>> _evolver = nullptr,
                  double hbar = 1.0
                  );

        virtual const H_matrix_2D* hemiltonian_ptr() const {
            return &H;
        }        
        
        // update the hamiltonian matrix
        void update_H_x();
        void update_H_y();

        inline void update_H() {
            update_H_x();
            update_H_y();
        } // do both

        // potential (x,y), TODO define it
        //double V(double x, double y) const;

        // set 0 on boundaries
        void boundaries_setup();
        
        // change the hemiltonian expression
        virtual void set_mass(double) override;

        // allow to set boundaries
        virtual void post(double) override;
        
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
        
        // override these functions
        void replace_wave(const wave_vector& other, size_t M);
        void replace_wave(const std::function<qsim::wave_t (double, double)>&, size_t N, size_t M);

        void replace_wave(wave_vector&& other, size_t M);

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

            inline size_t k() const {
                return sys.map(i,j);
            }
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

            inline size_t k() const {
                return sys.map(i,j);
            }
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
