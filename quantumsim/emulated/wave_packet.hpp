#pragma once

#include "region.hpp"

namespace qsim::emu {

    class observable;

    class wave_packet {
        
        // packet energy
        double _energy;

        std::shared_ptr<region> map; 
        std::shared_ptr<potential<size_t>> V; 
        std::vector<double> A;
        std::vector<double> B;


    public:
        
        wave_packet(double energy, std::shared_ptr<region> map, std::shared_ptr<potential<size_t>> V);

        /*
         * iteration
         */
        struct const_iterator {
            size_t k;
            wave_packet& wave;

            const_iterator(wave_packet&, size_t);

            const_iterator operator++();
            const_iterator operator++(int);
            bool operator!=(iterator);
        };

        const_iterator begin() const;
        const_iterator end() const;

        // regions
        inline size_t size() const {
            return V->size();
        }
    
        // lambda coefficient (complex)
        qsim::wave_t damp(size_t k) const;
        qsim::wave_t damp(double x) const;
        
        // packet energy setter
        void set_energy(double);
        double energy() const;

        /*
         * Access to A and B
         */
        std::pair<double, double> coefficients(size_t k) const;
        std::pair<double, double> coefficients(double x) const;

        inline std::pair<double, double> operator[](size_t k) const {
            return coefficients(k);
        }
        
        /*
         * determine A and B in function of the potential
         * TODO, technical part
         */
        void eval_coefficients();

        // function access
        double operator(double x) const;

        // integrals
        double position() const;
        double momentum() const;
        // custom observable action
        double observe(observable*) const;
    };
}
