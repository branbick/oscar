#include "biquad_notch_filter.hpp"

#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        if (argc == 4)
        {
            // Set (via constructor), get, and print damping ratios
            const BiquadNotchFilter kBiquadNotchFilter{std::stod(argv[1]),
                                                       std::stod(argv[2]),
                                                       std::stod(argv[3])};
            const BiquadDampingRatios kDampingRatios {
                kBiquadNotchFilter.getDampingRatios()};
            std::cout << "zetaNum: " << kDampingRatios.numerator << std::endl;
            std::cout << "zetaDen: " << kDampingRatios.denominator
                << std::endl;

            return 0;
        }

        throw "Enter wc (rad/s), wb (rad/s), and d (dB)--in that order";
    }
    catch(const char* const kExceptionMsg)
    {
        std::cerr << "ERROR: " << kExceptionMsg << std::endl;
        return 1;
    }
}
