#include "sys_id_sine_sweep.hpp"

#include <cmath>
#include <numeric>

constexpr double gkPi {3.14159265358979323846};

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

    // Calculate and set the values of the middle elements. Note that the
    // second argument of pow is the logarithmic step size.
    const int kNumValsMinus1 {kNumVals - 1};
    const double kConst {std::pow(10.0, (std::log10(kMaxVal)
        - std::log10(kMinVal)) / kNumValsMinus1)};
    for (int i {1}; i < kNumValsMinus1; i++)
        logSpacedVec.at(i) = logSpacedVec.at(i - 1) * kConst;

    return logSpacedVec;
}

std::vector<int> calcSamplesPerFreq(const std::vector<double>& kAngFreqs,
                                    const int kCyclesPerFreq,
                                    const double kSamplingFreq)
{
    // TODO: Check for invalid parameters based on the following requirements:
    // 1. kSamplingFreq >= 2 * kAngFreqs.back() (Nyquist-Shannon sampling
    //    theorem)
    // 2. kCyclesPerFreq > 1

    // In words, the number of samples for kAngFreqs.at(i) equals the floor of
    // the following quantity, plus one: the period corresponding to
    // kAngFreqs.at(i) [2 * gkPi / kAngFreqs.at(i)], divided by the sampling
    // period (1 / kSamplingFreq), multiplied by the number of cycles--where
    // the latter two terms are constant across kAngFreqs.
    const int kNumFreqs {static_cast<int>(kAngFreqs.size())};
    std::vector<int> samplesPerFreq(kNumFreqs);
    const double kConst {2 * gkPi * kSamplingFreq
        * kCyclesPerFreq};  // (rad/s)
    for (int i {0}; i < kNumFreqs; i++)
        samplesPerFreq.at(i) = static_cast<int>(std::floor(kConst
            / kAngFreqs.at(i))) + 1;

    return samplesPerFreq;
}

std::vector<double> generateInputSignal(
    const double kAmplitude,
    const std::vector<double>& kAngFreqs,
    const std::vector<int>& kSamplesPerFreq,
    const double kSamplingPeriod)
{
    std::vector<double> inputSignal(std::accumulate(kSamplesPerFreq.begin(),
        kSamplesPerFreq.end(), 0));

    // Calculate the elemental values of inputSignal by progressing through
    // kAngFreqs, advancing to kAngFreqs.at(freqIndex + 1) once the number of
    // samples collected for kAngFreqs.at(freqIndex)--i.e., numSamples--equals
    // kSamplesPerFreq.at(freqIndex). Note that time also corresponds to
    // kAngFreqs.at(freqIndex), and resetting it to 0.0 once the aforementioned
    // equivalence is reached ensures each "new" sinusoid starts with a range
    // value of 0.0.
    int freqIndex {0};
    double time {0.0};  // (s)
    int numSamples {0};
    for (double& rangeVal : inputSignal)
    {
        rangeVal = kAmplitude * std::sin(kAngFreqs.at(freqIndex) * time);
        time += kSamplingPeriod;
        if (++numSamples == kSamplesPerFreq.at(freqIndex))
        {
            freqIndex++;
            time = 0.0;
            numSamples = 0;
        }
    }

    return inputSignal;
}

FreqResponse calcMagAndPhase(const std::vector<double>& kOutputSignal,
                             const double kAmplitude,
                             const std::vector<double>& kAngFreqs,
                             const std::vector<int>& kSamplesPerFreq,
                             const double kSamplingPeriod,
                             const int kCyclesToIgnorePerFreq)
{
    // TODO: Check the validity of kCyclesToIgnorePerFreq: It must be > 0

    const double kConst1 {2 * gkPi / kSamplingPeriod};  // (rad/s)
    const double kConst2 {kConst1 * kCyclesToIgnorePerFreq};  // (rad/s)
    int startSampleIndex {0};  // Starting index for kOutputSignal
    const int kNumFreqs {static_cast<int>(kAngFreqs.size())};
    FreqResponse freqResponse {std::vector<double>(kNumFreqs),
        std::vector<double>(kNumFreqs)};  // Copy elision
    constexpr double kRadToDeg {180 / gkPi};  // (deg/rad)
    int samplesThusFar {0};  // Counter to advance startSampleIndex
    for (int i {0}; i < kNumFreqs; i++)
    {
        // Calculate the number of samples to 1) collect for one cycle and 2)
        // ignore
        const double kAngFreq {kAngFreqs.at(i)};  // (rad/s)
        const int kSamplesForOneCycle {static_cast<int>(std::round(kConst1
            / kAngFreq)) + 1};
        const int kSamplesToIgnore {static_cast<int>(std::round(kConst2
            / kAngFreq))};
        // TODO: Before proceeding, ensure ...
        // 1. kSamplesToIgnore < kSamplesPerFreq.at(i) (to prevent the entire
        //    portion of kOutputSignal corresponding to kAngFreq from being
        //    ignored)
        // 2. samplesThusFar + kSamplesToIgnore + kSamplesForOneCycle - 1 <
        //    kOutputSignal.size() [to prevent kOutputSignal.at(...) from
        //    reporting an out-of-range error in the for loop below*]
        //
        // * This situation could be avoided by flooring instead of rounding in
        // the calculations of both kSamplesForOneCycle and kSamplesToIgnore
        // above, but doing that would decrease the accuracy of freqResponse.

        // Calculate the integrands of the first in-phase and quadrature
        // Fourier-series coefficients--i.e., b1 and a1, respectively
        std::vector<double> inPhaseIntegrand1(kSamplesForOneCycle);
        std::vector<double> quadratureIntegrand1(kSamplesForOneCycle);
        startSampleIndex += kSamplesToIgnore;
        for (int j {0}; j < kSamplesForOneCycle; j++)
        {
            // j * kSamplingPeriod == time (s)
            inPhaseIntegrand1.at(j) = kOutputSignal.at(startSampleIndex + j)
                * std::sin(kAngFreq * j * kSamplingPeriod);
            quadratureIntegrand1.at(j) = kOutputSignal.at(startSampleIndex + j)
                * std::cos(kAngFreq * j * kSamplingPeriod);
        }

        // Calculate b1 and a1
        const double kInPhaseCoeff1 {kAngFreq / gkPi * trapezoidalRule(
            inPhaseIntegrand1, kSamplingPeriod)};
        const double kQuadratureCoeff1 {kAngFreq / gkPi * trapezoidalRule(
            quadratureIntegrand1, kSamplingPeriod)};

        // Calculate the frequency response (magnitude and phase)
        freqResponse.magnitude.at(i) = 20 * std::log10(std::sqrt(kInPhaseCoeff1
            * kInPhaseCoeff1 + kQuadratureCoeff1 * kQuadratureCoeff1)
            / kAmplitude);
        freqResponse.phase.at(i) = kRadToDeg * std::atan2(kQuadratureCoeff1,
            kInPhaseCoeff1);

        // Prepare for the next iteration
        startSampleIndex = (samplesThusFar += kSamplesPerFreq.at(i));
    }

    return freqResponse;
}

double trapezoidalRule(const std::vector<double>& kVals,
                       const double kStepSize)
{
    // TODO: Check for invalid parameters based on the following requirements:
    // 1. kVals.size() > 1
    // 2. kStepSize > 0.0

    double integral {kStepSize / 2 * kVals.front()};
    const int kNumValsMinus1 {static_cast<int>(kVals.size()) - 1};
    for (int i {1}; i < kNumValsMinus1; i++)
        integral += kStepSize * kVals.at(i);
    integral += kStepSize / 2 * kVals.back();

    return integral;
}
