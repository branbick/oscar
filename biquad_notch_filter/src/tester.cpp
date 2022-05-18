// Build command: g++ tester.cpp biquad_notch_filter.cpp -Wall -Wextra -Wpedantic -std=c++17 -o tester.out

#include "biquad_notch_filter.hpp"

#include <iostream>
#include <string>

void printDampingRatios(const BiquadNotchFilter& kBiquadNotchFilter)
{
    const BiquadParams::DampingRatios kDampingRatios {
        kBiquadNotchFilter.getDampingRatios()};
    std::cout << "Damping ratios:" << std::endl;
    std::cout << "- num: " << kDampingRatios.num << std::endl;
    std::cout << "- den: " << kDampingRatios.den << std::endl;
}

void printCoefficients(const BiquadNotchFilter& kBiquadNotchFilter)
{
    const BiquadParams::Coefficients kCoefficients {
        kBiquadNotchFilter.getCoefficients()};
    std::cout << "Coefficients:" << std::endl;
    std::cout << "- b0: " << kCoefficients.b0 << std::endl;
    std::cout << "- b1: " << kCoefficients.b1 << std::endl;
    std::cout << "- b2: " << kCoefficients.b2 << std::endl;
    std::cout << "- a1: " << kCoefficients.a1 << std::endl;
    std::cout << "- a2: " << kCoefficients.a2 << std::endl;
}

int main(int argc, char* argv[])
{
    try
    {
        if (argc == 5)
        {
            BiquadNotchFilter biquadNotchFilter{std::stod(argv[1]),
                                                std::stod(argv[2]),
                                                std::stod(argv[3]),
                                                std::stod(argv[4])};

            printDampingRatios(biquadNotchFilter);
            printCoefficients(biquadNotchFilter);

            return 0;
        }

        throw "Enter wc (rad/s), wb (rad/s), d (+dB), and T (s)--in order";
    }
    catch (const char* const kExceptionMsg)
    {
        std::cerr << "ERROR: " << kExceptionMsg << std::endl;
        return 1;
    }
}
