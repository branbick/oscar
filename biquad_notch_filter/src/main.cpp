// TODO: For testing only. Delete this file.
// GCC options: -Wall -Wextra -Wpedantic -std=c++17

#include "biquad_notch_filter.hpp"

int main()
{
    BiquadNotchFilter biquadNotchFilter{120.0, 40.0, 30.0};
    biquadNotchFilter.printDampingRatios();

    return 0;
}
