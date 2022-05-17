#include "biquad_notch_filter.hpp"

#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        if (argc == 5)
        {
            // Set (via constructor), get, and print damping ratios
            BiquadNotchFilter biquadNotchFilter{std::stod(argv[1]),
                                                std::stod(argv[2]),
                                                std::stod(argv[3]),
                                                std::stod(argv[4])};
            const BiquadParams::DampingRatios kDampingRatios {
                biquadNotchFilter.getDampingRatios()};
            std::cout << "zetaNum: " << kDampingRatios.num << std::endl;
            std::cout << "zetaDen: " << kDampingRatios.den << std::endl;

            return 0;
        }

        throw "Enter--in order--wc (rad/s), wb (rad/s), d (+dB), and T (s)";
    }
    catch (const char* const kExceptionMsg)
    {
        std::cerr << "ERROR: " << kExceptionMsg << std::endl;
        return 1;
    }
}
