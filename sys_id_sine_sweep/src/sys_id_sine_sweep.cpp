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
                             const double kSamplingFreq,
                             const int kCyclesToIgnorePerFreq)
{
    // TODO: Clean up comments

    // Calculate number of samples to ignore
    const int kNumFreqs {static_cast<int>(kAngFreqs.size())};
    std::vector<int> samplesToIgnorePerFreq(kNumFreqs);
    const double kConst {
        2 * gkPi * kSamplingFreq * kCyclesToIgnorePerFreq};  // (rad/s)
    for (int i {0}; i < kNumFreqs; i++)
        samplesToIgnorePerFreq.at(i) = static_cast<int>(std::floor(kConst
            / kAngFreqs.at(i)));
    // TODO: Ensure that samplesToIgnorePerFreq.at(i) < kSamplesPerFreq.at(i)

    int dynamicSampleSize {0};
    const double kSamplingPeriod {1 / kSamplingFreq};  // (s)
    std::vector<double> magnitude(kNumFreqs);  // (dB)
    std::vector<double> phase(kNumFreqs);  // (deg)
    const double kRadToDeg {180 / gkPi};  // (deg/rad)
    for (int i {0}; i < kNumFreqs; i++)
    {
        const int kNumSamples {kSamplesPerFreq.at(i)};
        dynamicSampleSize += kNumSamples;
        const int kStartSampleIndex = dynamicSampleSize - kNumSamples
            + samplesToIgnorePerFreq.at(i);
        const double kAngFreq {kAngFreqs.at(i)};  // (rad/s)
        const int kIntegrandLength {dynamicSampleSize - kStartSampleIndex};
        std::vector<double> inPhaseIntegrand(kIntegrandLength);
        std::vector<double> quadratureIntegrand(kIntegrandLength);
        for (int j {kStartSampleIndex}; j < dynamicSampleSize; j++)
        {
            const int kIntegrandIndex {j - kStartSampleIndex};
            const double kOutputSignalRangeVal {kOutputSignal.at(j)};
            // j * kSamplingPeriod is the time (s)
            const double kTrigFxnArg {
                kAngFreq * j * kSamplingPeriod};  // (rad)

            // Calculate integrands of in-phase and quadrature Fourier-series
            // terms--b1 and a1, respectively
            inPhaseIntegrand.at(kIntegrandIndex) = kOutputSignalRangeVal
                * std::sin(kTrigFxnArg);
            quadratureIntegrand.at(kIntegrandIndex) = kOutputSignalRangeVal
                * std::cos(kTrigFxnArg);
        }

        // Calculate b1 and a1
        const double kFactor {2 / kSamplingPeriod};  // (s)
        const double kInPhaseTerm {kFactor * trapezoidalRule(inPhaseIntegrand,
            kSamplingPeriod)};
        const double kQuadratureTerm {kFactor * trapezoidalRule(
            quadratureIntegrand, kSamplingPeriod)};

        // Calculate magnitude and phase
        magnitude.at(i) = std::sqrt(kInPhaseTerm * kInPhaseTerm
            + kQuadratureTerm * kQuadratureTerm) / kAmplitude;
        phase.at(i) = kRadToDeg * std::atan2(kQuadratureTerm,
            kInPhaseTerm);
    }
    const FreqResponse kFreqResponse {magnitude, phase};

    return kFreqResponse;
}

double trapezoidalRule(const std::vector<double>& kVals,
                       const double kStepSize)
{
    const double kHalfStep {kStepSize / 2};
    double integral {kHalfStep * kVals.front()};
    const int kNumValsMinus1 {static_cast<int>(kVals.size()) - 1};
    for (int i {1}; i < kNumValsMinus1; i++)
        integral += kStepSize * kVals.at(i);
    integral += kHalfStep * kVals.back();

    return integral;
}
