// Refer to "How to calculate the damping ratios of a biquadratic notch
// filter," located at ../doc/damping_ratios.pdf

struct BiquadDampingRatios
{
    double numerator {0.0};
    double denominator {0.0};
};

class BiquadNotchFilter
{
public:
    BiquadNotchFilter(double kCenterFreq,
                      double kBandwidth,
                      double kNotchDepth);

    bool setDampingRatios(double kCenterFreq,
                          double kBandwidth,
                          double kNotchDepth);

    BiquadDampingRatios getDampingRatios() const;

private:
    BiquadDampingRatios dampingRatios_;
};
