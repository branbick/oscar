// Refer to the following white paper, located at ../doc: "How to perform
// system identification using the sine-sweep method" (sys_id_sine_sweep.pdf)

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
// PARAMETERS
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
// PARAMETERS
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
// PARAMETERS
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

// BRIEF
// calcMagAndPhase calculates the frequency response (magnitude and phase; see
// FreqResponse) of the stable, LTI, SISO system being "identified"--the one
// that output kOutputSignal in response to an input signal of amplitude
// kAmplitude, sequentially composed of kAngFreqs.size() different frequencies.
// Both the input and output signals had the same number of samples,
// kSamplesPerFreq, collected for each element of kAngFreqs; and both of those
// signals were sampled every kSamplingPeriod seconds. Also, for each element
// of kAngFreqs, the frequency response is calculated over only one cycle of
// kOutputSignal after ignoring kCyclesToIgnorePerFreq cycles, as the transient
// response of the system must be discarded to properly calculate its frequency
// response.
//
// PARAMETERS
// kOutputSignal
//     The sinusoidal signal sequentially composed of kAngFreqs.size()
//     different frequencies, output by the stable, LTI, SISO system being
//     "identified" [same unit(s) as that/those of kAmplitude]
// kAmplitude
//     The amplitude of the input signal [arbitrary unit(s)]
// kAngFreqs
//     The vector of angular frequencies both the input and output signals are
//     sequentially composed of (rad/s)
// kSamplesPerFreq
//     The number of samples collected--for both the input and output signals--
//     for each of the kAngFreqs.size() different frequencies that compose
//     kAngFreqs
// kSamplingPeriod
//     The sampling period for both the input and output signals, dependent on
//     the embedded system (s)
// kCyclesToIgnorePerFreq
//     The number of cycles of kOutputSignal to ignore for each of the
//     kAngFreqs.size() different frequencies that compose kAngFreqs before
//     calculating the frequency response; must be 1) less than the number of
//     cycles both the input and output signals complete for each element of
//     kAngFreqs minus one and 2) greater than or equal to the number of cycles
//     it takes the transient response to die out; constant across kAngFreqs
//
// RETURN VALUE
// The frequency response--i.e., magnitude (dB) and phase (deg)--of the now
// "identified" stable, LTI, SISO system
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
// PARAMETERS
// kVals
//     The values to integrate
// kStepSize
//     The step size / spacing between adjacent points in the domain
//
// RETURN VALUE
// The integral of kVals over a domain of uniformly spaced points
double trapezoidalRule(const std::vector<double>& kVals, double kStepSize);
