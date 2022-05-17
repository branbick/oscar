// Refer to the following white papers, located at ../doc:
// - "How to calculate the damping ratios of a biquadratic notch filter"
//   (damping_ratios.pdf)
// - "How to implement a digital biquadratic notch filter (in software)"
//   (digital_implementation.pdf)

struct BiquadParams
{
    struct DampingRatios
    {
        double num {0.0};  // "Numerator"
        double den {0.0};  // "Denominator"
    };

    struct Coefficients
    {
        double b0 {0.0};
        double b1 {0.0};
        double b2 {0.0};
        double a1 {0.0};
        double a2 {0.0};
    };

    struct States           // Based on a direct-form-I realization
    {
        double out1 {0.0};  // "Output n-minus-1 / previous output"
        double out2 {0.0};  // "Output n-minus-2 / second-to-last output"
        double in1 {0.0};   // "Input n-minus-1 / previous input"
        double in2 {0.0};   // "Input n-minus-2 / second-to-last input"
                            // Where n := sample number
    };
};

class BiquadNotchFilter
{
public:
    BiquadNotchFilter(double kCenterFreq,       // (rad/s)
                      double kBandwidth,        // (rad/s)
                      double kNotchDepth,       // (+dB)
                      double kSamplingPeriod);  // (s)

    BiquadParams::DampingRatios getDampingRatios() const;
    BiquadParams::Coefficients getCoefficients() const;
    void resetStates();
    double filterSignal(double kIn0);

private:
    BiquadParams::DampingRatios dampingRatios_;
    BiquadParams::Coefficients coefficients_;
    BiquadParams::States states_;
};
