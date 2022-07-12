#include "sys_id_sine_sweep.hpp"

#include <cmath>
#include <numeric>

constexpr double gkPi {3.14159265358979323846};

std::vector<double> generateInputSignal(const double kAmplitude,
                                        const double kMinFreq,
                                        const double kMaxFreq,
                                        const int kNumFreqs,
                                        const int kCyclesPerFreq,
                                        const double kSamplingFreq)
{
    const std::vector<double> kFreqVec {logSpace(kMinFreq, kMaxFreq,
        kNumFreqs)};  // (rad/s)

    // Calculate number of samples for each element of kFreqVec
    std::vector<int> numSamplesVec(kNumFreqs);
    for (int i {0}; i < kNumFreqs; i++)
        numSamplesVec.at(i) = static_cast<int>(std::floor(2 * gkPi
            / kFreqVec.at(i) * kSamplingFreq * kCyclesPerFreq));

    const int kNumSamplesTot {std::accumulate(numSamplesVec.begin(),
        numSamplesVec.end(), 0)};
    std::vector<double> inputSignal(kNumSamplesTot);

    // TODO: Calculate values of inputSignal by looping thru kFreqVec,
    // switching to kFreqVec.at(i + 1) once number of samples collected for
    // kFreqVec.at(i) == numSamplesVec.at(i)
    int freqIndex {0};
    int numSamples {0};  // For kFreqVec.at(freqIndex)
    for (int i {0}; i < kNumSamplesTot; i++)
    {
        inputSignal.at(i) = kAmplitude * std::sin(kFreqVec.at(freqIndex) * i
            / kSamplingFreq);

        numSamples++;
        if (numSamples == numSamplesVec.at(freqIndex))
        {
            freqIndex++;
            numSamples = 0;
        }
    }

    return inputSignal;
}

std::vector<double> logSpace(const double kMinVal,
                             const double kMaxVal,
                             const int kNumVals)
{
    std::vector<double> logSpacedVec(kNumVals);

    // Set values of first and last elements
    logSpacedVec.front() = kMinVal;
    logSpacedVec.back() = kMaxVal;

    // Calculate and set value(s) of middle element(s)
    const double kLogStepSize {(std::log10(kMaxVal) - std::log10(kMinVal))
        / (kNumVals - 1)};
    for (int i {1}; i < kNumVals - 1; i++)
        logSpacedVec.at(i) = logSpacedVec.at(i - 1)
            * std::pow(10.0, kLogStepSize);

    return logSpacedVec;
}