// Refer to "How to calculate the damping ratios of a biquadratic notch
// filter," located at ../doc/damping_ratios.pdf

#include <iostream>  // TODO: For testing only. Delete this line.

class BiquadNotchFilter
{
public:
    BiquadNotchFilter(double kCenterFreq,
                      double kBandwidth,
                      double kNotchDepth);

    void setDampingRatios(double kCenterFreq,
                          double kBandwidth,
                          double kNotchDepth);

    // TODO: For testing only. Delete this function.
    void printDampingRatios() const
    {
        std::cout << "- zetaNum: " << dampingRatios_.numerator << std::endl;
        std::cout << "- zetaDen: " << dampingRatios_.denominator << std::endl;
    }
private:
    struct DampingRatios
    {
        double numerator {0.0};
        double denominator {0.0};
    } dampingRatios_;
};
