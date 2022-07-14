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
    // TODO: Compare kMaxFreq and kSamplingFreq to check if the Nyquist-Shannon
    // sampling theorem is adhered to
    const std::vector<double> kFreqVec {logSpace(kMinFreq, kMaxFreq,
        kNumFreqs)};  // (rad/s)

    // Calculate the number of samples collected for each element of kFreqVec.
    // Note that the rvalue in the for statement (body) is the period
    // corresponding to kFreqVec[i] [i.e., 2 * gkPi / kFreqVec.at(i)] divided
    // by the sampling period multiplied by the number of cycles.
    std::vector<int> numSamplesVec(kNumFreqs);
    const double kSamplingPeriod {1 / kSamplingFreq};  // (s)
    const double kConst {2 * gkPi / kSamplingPeriod * kCyclesPerFreq};  // (Hz)
    for (int i {0}; i < kNumFreqs; i++)
        numSamplesVec.at(i) = static_cast<int>(std::floor(kConst
            / kFreqVec.at(i)));

    // - Increment the last element by 1 to append a terminating value of 0.0
    //   to inputSignal (see below)
    numSamplesVec.back()++;

    std::vector<double> inputSignal(std::accumulate(numSamplesVec.begin(),
        numSamplesVec.end(), 0));

    // Calculate the elemental values of inputSignal by progressing through
    // kFreqVec, advancing to kFreqVec[i + 1] once the number of samples
    // collected for kFreqVec[i] equals numSamplesVec[i]. Note that both time
    // and numSamples correspond to kFreqVec[i], and resetting time to 0.0 once
    // the aforementioned equivalence is reached ensures each "new" sinusoid
    // starts with a range value of 0.0.
    int freqIndex {0};
    double time {0.0};  // (s)
    int numSamples {0};
    for (double& rangeVal : inputSignal)
    {
        rangeVal = kAmplitude * std::sin(kFreqVec.at(freqIndex) * time);
        time += kSamplingPeriod;
        numSamples++;
        if (numSamples == numSamplesVec.at(freqIndex))
        {
            freqIndex++;
            time = 0.0;
            numSamples = 0;
        }
    }

    // - Override the last element: terminate the range values with 0.0
    inputSignal.back() = 0.0;

    return inputSignal;
}

std::vector<double> logSpace(const double kMinVal,
                             const double kMaxVal,
                             const int kNumVals)
{
    // TODO: Check for invalid parameters based on the following requirements:
    // 1. kMaxVal > kMinVal > 0.0
    // 2. kNumVals > 2
    std::vector<double> logSpacedVec(kNumVals);

    // Set the values of the first and last elements
    logSpacedVec.front() = kMinVal;
    logSpacedVec.back() = kMaxVal;

    // Calculate and set the value(s) of the middle element(s). Note that the
    // second argument of pow is the logarithmic step size.
    const double kConst {std::pow(10.0, (std::log10(kMaxVal)
        - std::log10(kMinVal)) / (kNumVals - 1))};
    for (int i {1}; i < kNumVals - 1; i++)
        logSpacedVec.at(i) = logSpacedVec.at(i - 1) * kConst;

    return logSpacedVec;
}

double trapezoidalRule(const std::vector<double>& kVals,
                       const double kStepSize)
{
    double integral {kStepSize / 2 * kVals.front()};
    const int kNumVals {static_cast<int>(kVals.size())};
    for (int i {1}; i < kNumVals - 1; i++)
        integral += kStepSize * kVals.at(i);
    integral += kStepSize / 2 * kVals.back();

    return integral;
}
