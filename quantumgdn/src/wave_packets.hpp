#pragma once

#include <Godot.hpp>
#include <Reference.hpp>
#include <functional>
#include "quantumsim/grid/fw.hpp"

namespace godot {

    class grid_wave;
    
    class wave_packet : public Reference {
        GODOT_CLASS(wave_packet, Reference)

        typedef std::function<qsim::grid::wave_t (size_t)> Op;
        Op op; // operation

   protected:

        size_t N;

        wave_packet(Op&& _op, size_t _N) : op(std::move(_op)), N(_N) {}

   public:

        explicit wave_packet();
        virtual ~wave_packet();

        class Op_iter {

            size_t k;
            const Op& op;

        public:

            Op_iter(const Op& op, size_t _k);

            qsim::grid::wave_t operator*() const;
            bool operator!=(const Op_iter& other) const;
            Op_iter& operator++();
        };

        Op_iter begin() const;
        Op_iter end() const;
        
        // this reference will automaticly auto-delete
        Ref<grid_wave> generate() const;

        void _init();
        static void _register_methods();
    };

    class gaussian_packet : public wave_packet {
        GODOT_CLASS(gaussian_packet, wave_packet)

        double first;
        double L;
        double k0;
        double x0;
        double sigma;

    public:
 
        gaussian_packet(size_t _N = 50,
                        double _first = -1.0, 
                        double _second = 1.0, 
                        double n = 0.0, 
                        double _x0 = 0.0, 
                        double _sigma = 1.0
                       );

        ~gaussian_packet();
                
        qsim::grid::wave_t operator()(size_t k) const;

        void set_second(double);
        double get_second() const;

        void set_n(double n);
        double get_n() const;

        void _init();
        static void _register_methods();
    };
}
