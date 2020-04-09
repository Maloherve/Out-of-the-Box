#pragma once

namespace qsim::emu {

    class observable {
        virtual double observe(const wave_packet&) const = 0;
    };
}
