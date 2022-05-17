#include "biquad_notch_filter.hpp"

#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        if (argc == 5)
        {
            // Construct filter
            BiquadNotchFilter biquadNotchFilter{std::stod(argv[1]),
                                                std::stod(argv[2]),
                                                std::stod(argv[3]),
                                                std::stod(argv[4])};

            // Get and print damping ratios
            const BiquadParams::DampingRatios kDampingRatios {
                biquadNotchFilter.getDampingRatios()};
            std::cout << "Damping ratios:" << std::endl;
            std::cout << "- num: " << kDampingRatios.num << std::endl;
            std::cout << "- den: " << kDampingRatios.den << std::endl;

            // Get and print coefficients
            const BiquadParams::Coefficients kCoefficients {
                biquadNotchFilter.getCoefficients()};
            std::cout << std::endl << "Coefficients:" << std::endl;
            std::cout << "- b0: " << kCoefficients.b0 << std::endl;
            std::cout << "- b1: " << kCoefficients.b1 << std::endl;
            std::cout << "- b2: " << kCoefficients.b2 << std::endl;
            std::cout << "- a1: " << kCoefficients.a1 << std::endl;
            std::cout << "- a2: " << kCoefficients.a2 << std::endl;

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
