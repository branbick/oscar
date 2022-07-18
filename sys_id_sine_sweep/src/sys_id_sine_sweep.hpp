#include <vector>

struct FreqResponse
{
    std::vector<double> magnitude;  // (dB)
    std::vector<double> phase;      // (deg)
};

// BRIEF
// logSpace generates a logarithmically (base-10) spaced vector with kNumVals
// elements between kMinVal and kMaxVal, inclusive.
//
// PARAMETER(S)
// kMinVal
//     The minimum value / first element of the generated vector [arbitrary
//     unit(s)]
// kMaxVal
//     The maximum value / last element of the generated vector [same unit(s)
//     as that/those of kMinVal]
// kNumVals
//     The number of elements the generated vector contains
//
// RETURN VALUE
// A logarithmically spaced vector
std::vector<double> logSpace(double kMinVal, double kMaxVal, int kNumVals);

// BRIEF
// calcSamplesPerFreq calculates the number of samples to collect for each of
// the kAngFreqs.size() different frequencies that compose kAngFreqs--which is
// the return value of logSpace(<minimum frequency>, <maximum frequency>,
// <number of frequencies>)--based on the latter vector, kCyclesPerFreq, and
// kSamplingFreq.
//
// PARAMETER(S)
// kAngFreqs
//     The vector of angular frequencies returned by logSpace(
//     <minimum frequency>, <maximum frequency>, <number of frequencies>)
//     (rad/s)
// kCyclesPerFreq
//     The number of cycles the input signal later generated using the return
//     value of this function--among other arguments (see generateInputSignal)
//     --completes for each of the kAngFreqs.size() different frequencies that
//     compose kAngFreqs; constant across kAngFreqs
// kSamplingFreq
//     The sampling frequency, dependent on the embedded system (Hz)
//
// RETURN VALUE
// The number of samples to collect for each element of kAngFreqs
std::vector<int> calcSamplesPerFreq(const std::vector<double>& kAngFreqs,
                                    int kCyclesPerFreq,
                                    double kSamplingFreq);

// BRIEF
// generateInputSignal generates a signal of the form u(t) = A*sin(w*t)--where
// u := input signal [same unit(s) as that/those of A], t := time (s), A :=
// amplitude (e.g., deg), and w := angular frequency (rad/s)--whose frequency
// (w) increases every kCyclesPerFreq (an argument of calcSamplesPerFreq)
// cycles. Altogether, the signal is composed of kAngFreqs.size() different
// frequencies that are the elements of the return value of logSpace(
// <minimum frequency>, <maximum frequency>, <number of frequencies>).
// Practically, the generated signal serves as the input to the stable, LTI
// (linear, time-invariant), SISO (single-input, single-output) system being
// "identified." Note that only the range of the input signal is returned; its
// domain (i.e., time) can be easily calculated/generated using
// kSamplingPeriod.
//
// PARAMETER(S)
// kAmplitude
//     The amplitude of the sinusoid [arbitrary unit(s)]
// kAngFreqs
//     The vector of angular frequencies returned by logSpace(
//     <minimum frequency>, <maximum frequency>, <number of frequencies>); the
//     same one used as an argument of calcSamplesPerFreq (rad/s)
// kSamplesPerFreq
//     The number of samples to collect for each of the kAngFreqs.size()
//     different frequencies that compose kAngFreqs; the return value of
//     calcSamplesPerFreq(kAngFreqs, <number of cycles per frequency>,
//     <sampling frequency>)
// kSamplingPeriod
//     The sampling period, dependent on the embedded system; the reciprocal of
//     kSamplingFreq--an argument of calcSamplesPerFreq (s)
//
// RETURN VALUE
// A sinusoidal signal sequentially composed of kAngFreqs.size() different
// frequencies, to input into the stable, LTI, SISO system being "identified"
std::vector<double> generateInputSignal(
    double kAmplitude,
    const std::vector<double>& kAngFreqs,
    const std::vector<int>& kSamplesPerFreq,
    double kSamplingPeriod);

// TODO: Add function description
// kAmplitude, kAngFreqs, kSamplesPerFreq, and kSamplingPeriod need to be same
// as those above. kCyclesToIgnorePerFreq corresponds to transient response
// that needs to be ignored to properly calculate frequency response.
FreqResponse calcMagAndPhase(const std::vector<double>& kOutputSignal,
                             double kAmplitude,
                             const std::vector<double>& kAngFreqs,
                             const std::vector<int>& kSamplesPerFreq,
                             double kSamplingPeriod,
                             int kCyclesToIgnorePerFreq);

// BRIEF
// trapezoidalRule calculates the integral of kVals over a domain of points
// that are uniformly spaced kStepSize unit(s) apart, using the composite
// trapezoidal rule.
//
// PARAMETER(S)
// kVals
//     The values to integrate
// kStepSize
//     The step size / spacing between adjacent points in the domain
//
// RETURN VALUE
// The integral of kVals over a domain of uniformly spaced points
double trapezoidalRule(const std::vector<double>& kVals, double kStepSize);
