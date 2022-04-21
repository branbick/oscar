#include "biquad_notch_filter.hpp"

#include <cmath>
#include <iostream>

BiquadNotchFilter::BiquadNotchFilter(const double kCenterFreq,
                                     const double kBandwidth,
                                     const double kNotchDepth)
{
    setDampingRatios(kCenterFreq, kBandwidth, kNotchDepth);
}

void BiquadNotchFilter::setDampingRatios(const double kCenterFreq,
                                         const double kBandwidth,
                                         const double kNotchDepth)
{
    try
    {
        // Check for invalid parameters
        if (kCenterFreq <= 0.0)
            throw "wc must be > 0 rad/s";
        if (kBandwidth <= 0.0)
            throw "wb must be > 0 rad/s";
        if (kCenterFreq - kBandwidth / 2 <= 0.0)
            throw "wc - wb/2 must be > 0 rad/s";
        if (kNotchDepth <= -20 * std::log10(1 / std::sqrt(2.0)))
            throw "d must be > -20*log10(1/sqrt(2)) dB";

        // All parameters are valid. Set damping ratios.
        const double kConst {(kBandwidth + std::sqrt(kBandwidth * kBandwidth
            + 4 * kCenterFreq * kCenterFreq)) / (2 * kCenterFreq)};
        const double kConstSq {kConst * kConst};
        dampingRatios_.denominator = std::sqrt((2 - kConstSq - 1 / kConstSq) /
            (4 * (2 * std::pow(10.0, -kNotchDepth / 10) - 1)));
        dampingRatios_.numerator = std::pow(10.0, -kNotchDepth / 20)
            * dampingRatios_.denominator;

        return;
    }
    catch(const char* const kExceptMsg)
    {
        std::cerr << "ERROR: " << kExceptMsg << std::endl;
        return;
    }
}
