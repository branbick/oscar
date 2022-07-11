#include "sys_id_sine_sweep.hpp"

#include <cmath>

// std::vector<double> generateInputSignal(const double kAmplitude,
//                                         const double kMinFreq,
//                                         const double kMaxFreq,
//                                         const int kNumFreqs,
//                                         const int kCyclesPerFreq,
//                                         const double kSamplingFreq)
// {
//     int numSamples {0};  // TODO: Figure out how to calculate
//     for (int i {0}; i < kNumFreqs; i++)
//     {
//         // TODO: Add number of samples for ith test freq
//     }
//     std::vector<double> inputSignal(numSamples);
// }

std::vector<double> logSpace(const double kMinVal,
                             const double kMaxVal,
                             const int kNumPts)
{
    std::vector<double> logSpacedVec(kNumPts);

    // Set values of first and last elements
    logSpacedVec.front() = kMinVal;
    logSpacedVec.back() = kMaxVal;

    // Calculate and set value(s) of middle element(s)
    const double kLogStepSize {(std::log10(kMaxVal) - std::log10(kMinVal))
        / (kNumPts - 1)};
    for (int i {1}; i < kNumPts - 1; i++)
    {
        logSpacedVec.at(i) = logSpacedVec.at(i - 1)
            * std::pow(10.0, kLogStepSize);
    }

    return logSpacedVec;
}