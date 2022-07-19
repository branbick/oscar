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

    // Calculate and set the value(s) of the middle element(s). Note that the
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
    // TODO: Compare kAngFreqs.back() and kSamplingFreq to check if the
    // Nyquist-Shannon sampling theorem is adhered to. Also, check
    // kCyclesPerFreq: it must be > 0.

    // In words, the number of samples for kAngFreqs.at(i) equals the period
    // corresponding to kAngFreqs.at(i) [2 * gkPi / kAngFreqs.at(i)] divided by
    // the sampling period (1 / kSamplingFreq) multiplied by the number of
    // cycles, where the latter two terms are constant across kAngFreqs.
    const int kNumFreqs {static_cast<int>(kAngFreqs.size())};
    std::vector<int> samplesPerFreq(kNumFreqs);
    const double kConst {
        2 * gkPi * kSamplingFreq * kCyclesPerFreq};  // (rad/s)
    for (int i {0}; i < kNumFreqs; i++)
        samplesPerFreq.at(i) = static_cast<int>(std::floor(kConst
            / kAngFreqs.at(i)));

    // Increment the last element by 1 to properly terminate the input signal
    // later generated using the return value of this function--among other
    // arguments. (See generateInputSignal.)
    samplesPerFreq.back()++;

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
    // kAngFreqs, advancing to kAngFreqs.at(i + 1) once the number of samples
    // collected for kAngFreqs.at(i)--i.e., numSamples--equals
    // kSamplesPerFreq.at(i). Note that time also corresponds to
    // kAngFreqs.at(i), and resetting it to 0.0 once the aforementioned
    // equivalence is reached ensures each "new" sinusoid starts with a range
    // value of 0.0.
    int freqIndex {0};
    double time {0.0};  // (s)
    int numSamples {0};
    for (double& rangeVal : inputSignal)
    {
        rangeVal = kAmplitude * std::sin(kAngFreqs.at(freqIndex) * time);
        time += kSamplingPeriod;
        numSamples++;
        if (numSamples == kSamplesPerFreq.at(freqIndex))
        {
            freqIndex++;
            time = 0.0;
            numSamples = 0;
        }
    }

    // - Override the last element to terminate the range values with 0.0
    inputSignal.back() = 0.0;

    return inputSignal;
}

FreqResponse calcMagAndPhase(const std::vector<double>& kOutputSignal,
                             const double kAmplitude,
                             const std::vector<double>& kAngFreqs,
                             const std::vector<int>& kSamplesPerFreq,
                             const double kSamplingPeriod,
                             const int kCyclesToIgnorePerFreq)
{
    // TODO: Clean up comments

    // Calculate the number of samples to ignore--which correspond to the
    // transient response of kOutputSignal--for each element of kAngFreqs
    const int kNumFreqs {static_cast<int>(kAngFreqs.size())};
    std::vector<int> samplesToIgnorePerFreq(kNumFreqs);
    const double kConst {
        2 * gkPi / kSamplingPeriod * kCyclesToIgnorePerFreq};  // (rad/s)
    for (int i {0}; i < kNumFreqs; i++)
        samplesToIgnorePerFreq.at(i) = static_cast<int>(std::floor(kConst
            / kAngFreqs.at(i)));
        // TODO: Ensure that samplesToIgnorePerFreq.at(i)
        // < kSamplesPerFreq.at(i) before proceeding

    // Calculate the frequency response: magnitude and phase
    int dynamicSampleSize {0};
    FreqResponse freqResponse {std::vector<double>(kNumFreqs),
        std::vector<double>(kNumFreqs)};  // Copy elision
    constexpr double kRadToDeg {180 / gkPi};  // (deg/rad)
    for (int i {0}; i < kNumFreqs; i++)
    {
        // Calculate the integrands of the first in-phase and quadrature
        // Fourier-series coefficients--i.e., b1 and a1, respectively
        dynamicSampleSize += kSamplesPerFreq.at(i);
        const int kStartSampleIndex = dynamicSampleSize - kSamplesPerFreq.at(i)
            + samplesToIgnorePerFreq.at(i);
        const double kAngFreq {kAngFreqs.at(i)};  // (rad/s)
        std::vector<double> inPhaseIntegrand1(dynamicSampleSize
            - kStartSampleIndex);
        std::vector<double> quadratureIntegrand1(dynamicSampleSize
            - kStartSampleIndex);
        for (int j {kStartSampleIndex}; j < dynamicSampleSize; j++)
        {
            // j * kSamplingPeriod == time (s)
            inPhaseIntegrand1.at(j - kStartSampleIndex) = kOutputSignal.at(j)
                * std::sin(kAngFreq * j * kSamplingPeriod);
            quadratureIntegrand1.at(j - kStartSampleIndex) =
                kOutputSignal.at(j) * std::cos(kAngFreq * j * kSamplingPeriod);
        }

        // Calculate b1 and a1
        const double kInPhaseCoeff1 {2 / kSamplingPeriod * trapezoidalRule(
            inPhaseIntegrand1, kSamplingPeriod)};
        const double kQuadratureCoeff1 {2 / kSamplingPeriod * trapezoidalRule(
            quadratureIntegrand1, kSamplingPeriod)};

        // Calculate the magnitude and phase
        freqResponse.magnitude.at(i) = 20 * std::log10(std::sqrt(kInPhaseCoeff1
            * kInPhaseCoeff1 + kQuadratureCoeff1 * kQuadratureCoeff1)
            / kAmplitude);
        freqResponse.phase.at(i) = kRadToDeg * std::atan2(kQuadratureCoeff1,
            kInPhaseCoeff1);
    }

    return freqResponse;
}

double trapezoidalRule(const std::vector<double>& kVals,
                       const double kStepSize)
{
    double integral {kStepSize / 2 * kVals.front()};
    const int kNumValsMinus1 {static_cast<int>(kVals.size()) - 1};
    for (int i {1}; i < kNumValsMinus1; i++)
        integral += kStepSize * kVals.at(i);
    integral += kStepSize / 2 * kVals.back();

    return integral;
}
