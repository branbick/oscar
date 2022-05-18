#include "biquad_notch_filter.hpp"

#include <string>
#include <cmath>
#include <iostream>

BiquadNotchFilter::BiquadNotchFilter(const double kCenterFreq,
                                     const double kBandwidth,
                                     const double kNotchDepth,
                                     const double kSamplingPeriod)
{
    try
    {
        // Check for invalid parameters
        std::string exceptionMsg;
        bool exceptionOccurred {true};

        if (kCenterFreq <= 0.0)
            exceptionMsg = "wc must be > 0 rad/s";
        else if (kBandwidth <= 0.0)
            exceptionMsg = "wb must be > 0 rad/s";
        else if (kCenterFreq - kBandwidth / 2 <= 0.0)
            exceptionMsg = "wc - wb/2 must be > 0 rad/s";
        else if (kNotchDepth <= -20 * std::log10(1 / std::sqrt(2.0)))
            exceptionMsg = "d must be > -20*log10(1/sqrt(2)) dB";
        else if (kSamplingPeriod <= 0.0)
            exceptionMsg = "T must be > 0 s";
        else
            exceptionOccurred = false;

        if (exceptionOccurred)
            throw (exceptionMsg += ". Filter not properly constructed.");
        else  // All parameters are valid
        {
            // Set damping ratios
            const double kConst {(kBandwidth + std::sqrt(kBandwidth
                * kBandwidth + 4 * kCenterFreq * kCenterFreq)) / (2
                * kCenterFreq)};
            const double kConstSq {kConst * kConst};

            dampingRatios_.den = std::sqrt((2 - kConstSq - 1 / kConstSq)
                / (4 * (2 * std::pow(10.0, -kNotchDepth / 10) - 1)));
            dampingRatios_.num = std::pow(10.0, -kNotchDepth / 20)
                * dampingRatios_.den;

            // Set coefficients
            const double kWcTimesT {kCenterFreq * kSamplingPeriod};
            const double kSinWcTimesT {std::sin(kWcTimesT)};
            const double kZetaNumTimesSinWcTimesT {dampingRatios_.num
                * kSinWcTimesT};
            const double kZetaDenTimesSinWcTimesT {dampingRatios_.den
                * kSinWcTimesT};

            const double kAlpha0 {1 + kZetaDenTimesSinWcTimesT};

            coefficients_.b0 = (1 + kZetaNumTimesSinWcTimesT) / kAlpha0;
            coefficients_.b1 = -2 * std::cos(kWcTimesT) / kAlpha0;
            coefficients_.b2 = (1 - kZetaNumTimesSinWcTimesT) / kAlpha0;
            coefficients_.a1 = coefficients_.b1;
            coefficients_.a2 = (1 - kZetaDenTimesSinWcTimesT) / kAlpha0;
        }
    }
    catch (const std::string kExceptionMsg)
    {
        std::cerr << "ERROR: " << kExceptionMsg << std::endl;
    }
}

double BiquadNotchFilter::filterSignal(const double kInput)
{
    const double kOutput {coefficients_.b0 * kInput + coefficients_.b1
        * states_.in1 + coefficients_.b2 * states_.in2 - coefficients_.a1
        * states_.out1 - coefficients_.a2 * states_.out2};

    // Update states
    states_.in2 = states_.in1;
    states_.in1 = kInput;
    states_.out2 = states_.out1;
    states_.out1 = kOutput;

    return kOutput;
}

void BiquadNotchFilter::resetStates()
{
    states_ = BiquadParams::States();
}

BiquadParams::DampingRatios BiquadNotchFilter::getDampingRatios() const
{
    return dampingRatios_;
}

BiquadParams::Coefficients BiquadNotchFilter::getCoefficients() const
{
    return coefficients_;
}

BiquadParams::States BiquadNotchFilter::getStates() const
{
    return states_;
}
