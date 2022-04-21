// TODO: For testing only. Delete this file.
// GCC options: -Wall -Wextra -Wpedantic -std=c++17

#include "biquad_notch_filter.hpp"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    try
    {
        if (argc == 4)
        {
            const double kCenterFreq {std::stod(argv[1])};
            const double kBandwidth {std::stod(argv[2])};
            const double kNotchDepth {std::stod(argv[3])};

            BiquadNotchFilter biquadNotchFilter{kCenterFreq, kBandwidth, kNotchDepth};
            biquadNotchFilter.printDampingRatios();

            return 0;
        }

        throw "Enter wc (rad/s), wb (rad/s), and d (dB)";
    }
    catch(const char* exception)
    {
        std::cerr << "ERROR: " << exception << std::endl;
        return 1;
    }
}
