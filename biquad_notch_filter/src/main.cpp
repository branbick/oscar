// TODO: For testing only. Delete this file.
// GCC options: -Wall -Wextra -Wpedantic -std=c++17

#include "biquad_notch_filter.hpp"

#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        if (argc == 4)
        {
            BiquadNotchFilter biquadNotchFilter{std::stod(argv[1]),
                                                std::stod(argv[2]),
                                                std::stod(argv[3])};
            biquadNotchFilter.printDampingRatios();

            return 0;
        }

        throw "Enter wc (rad/s), wb (rad/s), and d (dB)--in that order";
    }
    catch(const char* const kExceptMsg)
    {
        std::cerr << "ERROR: " << kExceptMsg << std::endl;
        return 1;
    }
}
