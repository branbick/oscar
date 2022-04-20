#include "biquad_notch_filter.hpp"

#include <cmath>

BiquadNotchFilter::BiquadNotchFilter(const double kCenterFreq,
                                     const double kBandwidth,
                                     const double kNotchDepth)
{
    // TODO: Check for invalid params (e.g., kNotchDepth must be > 0.0)
    setDampingRatios(kCenterFreq, kBandwidth, kNotchDepth);
}

void BiquadNotchFilter::setDampingRatios(const double kCenterFreq,
                                         const double kBandwidth,
                                         const double kNotchDepth)
{
    const double kConst {(kBandwidth + std::sqrt(kBandwidth * kBandwidth
        + 4 * kCenterFreq * kCenterFreq)) / (2 * kCenterFreq)};
    const double kConstSq {kConst * kConst};
    dampingRatios_.denominator = std::sqrt((2 - kConstSq - 1 / kConstSq) /
        (4 * (2 * std::pow(10.0, -kNotchDepth / 10) - 1)));
    dampingRatios_.numerator = std::pow(10.0, -kNotchDepth / 20)
        * dampingRatios_.denominator;
}
