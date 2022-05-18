// Build: g++ tester.cpp biquad_notch_filter.cpp -Wall -Wextra -Wpedantic -std=c++17 -o tester.out
// Execute: ./tester.out 100 20 50 0.001

#include "biquad_notch_filter.hpp"

#include <iostream>
#include <cmath>
#include <string>

void printDampingRatios(const BiquadNotchFilter& kBiquadNotchFilter)
{
    const BiquadParams::DampingRatios kDampingRatios {
        kBiquadNotchFilter.getDampingRatios()};
    std::cout << "Damping ratios:" << std::endl;
    std::cout << "- num: " << kDampingRatios.num << std::endl;
    std::cout << "- den: " << kDampingRatios.den << std::endl;
}

void printCoefficients(const BiquadNotchFilter& kBiquadNotchFilter)
{
    const BiquadParams::Coefficients kCoefficients {
        kBiquadNotchFilter.getCoefficients()};
    std::cout << "Coefficients:" << std::endl;
    std::cout << "- b0: " << kCoefficients.b0 << std::endl;
    std::cout << "- b1: " << kCoefficients.b1 << std::endl;
    std::cout << "- b2: " << kCoefficients.b2 << std::endl;
    std::cout << "- a1: " << kCoefficients.a1 << std::endl;
    std::cout << "- a2: " << kCoefficients.a2 << std::endl;
}

void printStates(const BiquadNotchFilter& kBiquadNotchFilter)
{
    const BiquadParams::States kStates {kBiquadNotchFilter.getStates()};
    std::cout << "States:" << std::endl;
    std::cout << "- out1: " << kStates.out1 << std::endl;
    std::cout << "- out2: " << kStates.out2 << std::endl;
    std::cout << "- in1 : " << kStates.in1 << std::endl;
    std::cout << "- in2 : " << kStates.in2 << std::endl;
}

double* createSinusoid(const int kN,     // Number of samples (--)
                       const double kA,  // Amplitude (arbitrary)
                       const double kW,  // Angular frequency (rad/s)
                       const double kT)  // Sampling period (s)
{
    double* const wave {new double[kN]};
    for (int i {0}; i < kN; i++)
        wave[i] = kA * std::sin(kW * i * kT);

    return wave;
}

void printSinusoid(const int kN,          // Number of samples (--)
                   const double kWave[])  // Sinusoid
{
    for (int i {0}; i < kN; i++)
        std::cout << kWave[i] << " ";
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    try
    {
        if (argc == 5)
        {
            // Construct filter
            const double kWc {std::stod(argv[1])};
            const double kT {std::stod(argv[4])};

            BiquadNotchFilter biquadNotchFilter{kWc,
                                                std::stod(argv[2]),
                                                std::stod(argv[3]),
                                                kT};

            // Print initial parameters
            printDampingRatios(biquadNotchFilter);
            printCoefficients(biquadNotchFilter);
            printStates(biquadNotchFilter);

            // Create (dynamically allocated) sinusoids
            const int kNumSamples {1000};
            double* wave1 {createSinusoid(kNumSamples, 5.0, kWc, kT)};
            double* wave2 {createSinusoid(kNumSamples, 2.0, 3 * kWc, kT)};

            // Print sinusoids
            std::cout << "wave1: "; printSinusoid(kNumSamples, wave1);
            std::cout << "wave2: "; printSinusoid(kNumSamples, wave2);

            // Superimpose sinusoids
            double waveSuper[kNumSamples];
            for (int i {0}; i < kNumSamples; i++)
                waveSuper[i] = wave1[i] + wave2[i];

            // Print resultant sinusoid
            std::cout << "waveSuper: "; printSinusoid(kNumSamples, waveSuper);

            // Filter resultant sinusoid
            double waveFilt[kNumSamples];
            for (int i {0}; i < kNumSamples; i++)
                waveFilt[i] = biquadNotchFilter.filterSignal(waveSuper[i]);

            // Print filtered sinusoid
            std::cout << "waveFilt: "; printSinusoid(kNumSamples, waveFilt);

            // Delete (dynamically allocated) sinusoids
            delete[] wave1;
            delete[] wave2;

            // Print final parameters
            printDampingRatios(biquadNotchFilter);
            printCoefficients(biquadNotchFilter);
            printStates(biquadNotchFilter);

            // Reset then print states
            biquadNotchFilter.resetStates();
            printStates(biquadNotchFilter);

            return 0;
        }

        throw "Enter wc (rad/s), wb (rad/s), d (+dB), and T (s)--in order";
    }
    catch (const char* const kExceptionMsg)
    {
        std::cerr << "ERROR: " << kExceptionMsg << std::endl;
        return 1;
    }
}
