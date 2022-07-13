#include <vector>

// BRIEF
// generateInputSignal generates the signal to input into the stable, LTI
// system. The form of the generated signal is u(t) = A*sin(w*t), where u :=
// input signal (e.g., deg), t := time (s), A := amplitude (same as that/those
// of u), and w := angular frequency (rad/s). Units: kMinFreq and kMaxFreq:
// rad/s; kSamplingFreq: Hz. (Only the range is returned. The domain can be
// easily calculated using the sampling frequency.)
// TODO: Reqmt: kSamplingFreq vs. kMaxFreq--Nyquist sampling theorem
std::vector<double> generateInputSignal(double kAmplitude,
                                        double kMinFreq,
                                        double kMaxFreq,
                                        int kNumFreqs,
                                        int kCyclesPerFreq,
                                        double kSamplingFreq);

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
