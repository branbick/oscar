#include <vector>

// BRIEF
// logSpace generates a logarithmically (base-10) spaced vector with kNumVals
// values between kMinVal and kMaxVal, inclusive.
//
// PARAMETER(S)
// kMinVal
//     The minimum value / first element of the generated vector
// kMaxVal
//     The maximum value / last element of the generated vector
// kNumVals
//     The number of elements the generated vector contains
//
// RETURN VALUE
// A logarithmically (base-10) spaced vector with kNumVals values between
// kMinVal and kMaxVal, inclusive
std::vector<double> logSpace(double kMinVal, double kMaxVal, int kNumVals);

// TODO: Complete function description
// Calculate the number of samples collected for each element of kAngFreqs
std::vector<int> calcSamplesPerFreq(const std::vector<double>& kAngFreqs,
                                    int kCyclesPerFreq,
                                    double kSamplingFreq);

// TODO: Update function description
// BRIEF
// generateInputSignal generates a signal of the form u(t) = A*sin(w*t)--where
// u := input signal [same unit(s) as that/those of A], t := time (s), A :=
// amplitude (e.g., deg), and w := angular frequency (rad/s)--whose frequency
// (w) increases every kCyclesPerFreq cycles. Altogether, the signal is
// composed of kNumFreqs different frequencies that are precisely the elements
// of the return value of logSpace(kMinFreq, kMaxFreq, kNumFreqs). Practically,
// the generated signal is the input to the stable, LTI (linear, time-
// invariant), SISO (single-input, single-output) system being "identified."
// Note that only the range of the input signal is returned; its domain (i.e.,
// time) can be easily calculated/generated using kSamplingFreq.
//
// PARAMETER(S)
// kAmplitude
//     The amplitude of the sinusoid [arbitrary unit(s)]
// kMinFreq
//     The minimum--and first--angular frequency of the sinusoid (rad/s)
// kMaxFreq
//     The maximum--and last--angular frequency of the sinusoid (rad/s)
// kNumFreqs
//     The number of different frequencies that compose the sinusoid
// kCyclesPerFreq
//     The number of cycles the sinusoid completes for each of the kNumFreqs
//     different frequencies
// kSamplingFreq
//     The sampling frequency, dependent on the embedded system (Hz)
//
// RETURN VALUE
// A sinusoidal signal of amplitude kAmplitude; composed of kNumFreqs different
// frequencies ranging from kMinFreq rad/s to kMaxFreq rad/s--kCyclesPerFreq
// cycles for each; to input into the stable, LTI, SISO system being
// "identified"; sampled at kSamplingFreq Hz
std::vector<double> generateInputSignal(
    double kAmplitude,
    const std::vector<double>& kAngFreqs,
    const std::vector<int>& kSamplesPerFreq,
    double kSamplingPeriod);

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
// The integral of kVals over a domain with uniform spacing established by
// kStepSize, calculated using the composite trapezoidal rule
double trapezoidalRule(const std::vector<double>& kVals, double kStepSize);
