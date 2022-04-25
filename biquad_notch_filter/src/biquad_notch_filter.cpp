#include "biquad_notch_filter.hpp"

#include <string>
#include <cmath>
#include <iostream>

BiquadNotchFilter::BiquadNotchFilter(const double kCenterFreq,
                                     const double kBandwidth,
                                     const double kNotchDepth)
{
    setDampingRatios(kCenterFreq, kBandwidth, kNotchDepth);
}

bool BiquadNotchFilter::setDampingRatios(const double kCenterFreq,
                                         const double kBandwidth,
                                         const double kNotchDepth)
{
    try
    {
        // Check for invalid parameters
        std::string exceptionMsg;
        bool exceptionOccurred {false};

        if (kCenterFreq <= 0.0)
        {
            exceptionMsg = "wc must be > 0 rad/s";
            exceptionOccurred = true;
        }
        else if (kBandwidth <= 0.0)
        {
            exceptionMsg = "wb must be > 0 rad/s";
            exceptionOccurred = true;
        }
        else if (kCenterFreq - kBandwidth / 2 <= 0.0)
        {
            exceptionMsg = "wc - wb/2 must be > 0 rad/s";
            exceptionOccurred = true;
        }
        else if (kNotchDepth <= -20 * std::log10(1 / std::sqrt(2.0)))
        {
            exceptionMsg = "d must be > -20*log10(1/sqrt(2)) dB";
            exceptionOccurred = true;
        }

        if (exceptionOccurred)
            throw (exceptionMsg += ". Damping ratios not properly set.");

        // All parameters are valid. Properly set damping ratios.
        const double kConst {(kBandwidth + std::sqrt(kBandwidth * kBandwidth
            + 4 * kCenterFreq * kCenterFreq)) / (2 * kCenterFreq)};
        const double kConstSq {kConst * kConst};
        dampingRatios_.denominator = std::sqrt((2 - kConstSq - 1 / kConstSq) /
            (4 * (2 * std::pow(10.0, -kNotchDepth / 10) - 1)));
        dampingRatios_.numerator = std::pow(10.0, -kNotchDepth / 20)
            * dampingRatios_.denominator;

        return true;
    }
    catch (const std::string kExceptionMsg)
    {
        std::cerr << "ERROR: " << kExceptionMsg << std::endl;
        return false;
    }
}

BiquadDampingRatios BiquadNotchFilter::getDampingRatios() const
{
    return dampingRatios_;
}
