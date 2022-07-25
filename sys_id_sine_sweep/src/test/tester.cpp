// Build: g++ tester.cpp ../sys_id_sine_sweep.cpp -Wall -Wextra -Wpedantic -std=c++17 (-D PRINT_TEST#) -o tester.out
// Execute: ./tester.out

#include "../sys_id_sine_sweep.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>

int main()
{

    // Inputs
    constexpr double kMinFreq {10.0};  // (rad/s)
    constexpr double kMmaxFreq {100.0};  // (rad/s)
    constexpr int kNumFreqs {5};

    constexpr int kCyclesPerFreq {5};
    constexpr double kSamplingFreq {50.0};  // Must be >= kMmaxFreq / pi (Hz)

    constexpr double kAmplitude {0.5};

    constexpr int kCyclesToIgnorePerFreq {2};  // Must be < kCyclesPerFreq

    // Generate and optionally print a logarithmically spaced angular frequency
    // vector
    const std::vector<double> kAngFreqs {logSpace(kMinFreq, kMmaxFreq,
        kNumFreqs)};  // (rad/s)

#ifdef PRINT_TEST1
    std::cout << "logSpace test:" << std::endl;
    for (const double kAngFreq : kAngFreqs)
        std::cout << kAngFreq << " ";
    std::cout << std::endl;
#endif

    // Calculate and optionally print the number of samples for each element of
    // kAngFreqs
    const std::vector<int> kSamplesPerFreq {calcSamplesPerFreq(kAngFreqs,
        kCyclesPerFreq, kSamplingFreq)};

#ifdef PRINT_TEST2
    std::cout << "calcSamplesPerFreq test:" << std::endl;
    for (const int kNumSamples : kSamplesPerFreq)
        std::cout << kNumSamples << " ";
    std::cout << std::endl;
#endif

    // Generate and optionally print (to a .csv file) a corresponding input
    // signal
    constexpr double kSamplingPeriod {1 / kSamplingFreq};  // (s)
    const std::vector<double> kInputSignal {generateInputSignal(kAmplitude,
        kAngFreqs, kSamplesPerFreq, kSamplingPeriod)};

#ifdef PRINT_TEST3
    std::ofstream inputSignalData {"signal_data/input.csv"};
    const int kInputSignalSizeMinus1 {static_cast<int>(kInputSignal.size())
        - 1};
    for (int i {0}; i < kInputSignalSizeMinus1; i++)
        inputSignalData << kInputSignal.at(i) << ", ";
    inputSignalData << kInputSignal.back();
    inputSignalData.close();
#endif

    // Calculate and optionally print the corresponding frequency response:
    // magnitude and phase
    //
    // MATLAB code:
    // input = csvread('input.csv');
    // fs = 50;  % (Hz)
    // t = [0 : length(input) - 1] / fs;  % (s)
    // wn = 100;  % (rad/s)
    // zeta = 0.5;
    // s = tf('s'); G = wn^2 / (s^2 + 2*zeta*wn*s + wn^2);
    // output = lsim(G, input, t)';
    // fID = fopen('output.dat', 'a');
    // for i = 1:length(t), fprintf(fID, '%.6f ', output(i)); end, fclose(fID);
    // plot(t, input), xlabel('Time (s)'), ylabel('Signal')
    // hold on; plot(t, output); legend('in', 'out');
    // figure; bode(G)
    std::ifstream outputSignalData {"signal_data/output.dat"};
    const int kInputSignalSize {static_cast<int>(kInputSignal.size())};
    std::vector<double> outputSignal(kInputSignalSize);
    int outputSignalIndex {0};
    while (outputSignalData && outputSignalIndex < kInputSignalSize)
    {
        double rangeVal;
        outputSignalData >> rangeVal;
        outputSignal.at(outputSignalIndex) = rangeVal;
        outputSignalIndex++;
    }
    outputSignalData.close();
    const FreqResponse kFreqResponse {calcMagAndPhase(outputSignal, kAmplitude,
        kAngFreqs, kSamplesPerFreq, kSamplingPeriod, kCyclesToIgnorePerFreq)};

#ifdef PRINT_TEST4
    std::cout << "calcMagAndPhase test:" << std::endl;
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "- frequency (rad/s): ";
    for (const double kAngFreq : kAngFreqs)
        std::cout << std::setw(9) << kAngFreq << " ";
    std::cout << std::endl << "- magnitude (dB)   : ";
    for (const double kMag : kFreqResponse.magnitude)
        std::cout << std::setw(9) << kMag << " ";
    std::cout << std::endl << "- phase     (deg)  : ";
    for (const double kPhase : kFreqResponse.phase)
        std::cout << std::setw(9) << kPhase << " ";
    std::cout << std::endl;
#endif

    // Numerically compute the integral of the function below using the
    // (composite) trapezoidal rule and optionally print the result
    //
    // f(t) = 2*t^2 - 5*t + 13 for t = [0.0, 10.0] s and T = 0.1 s
    //
    // MATLAB code:
    // t = [0.0:0.1:10.0];  % (s)
    // f = 2*t.^2 - 5*t + 13;
    // for i = 1:length(t), fprintf('%.2f, ', f(i)), end, fprintf('\n')
    // trapz(t, f)
    std::vector<double> f {13.00, 12.52, 12.08, 11.68, 11.32, 11.00, 10.72, 10.48, 10.28, 10.12, 10.00, 9.92, 9.88, 9.88, 9.92, 10.00, 10.12, 10.28, 10.48, 10.72, 11.00, 11.32, 11.68, 12.08, 12.52, 13.00, 13.52, 14.08, 14.68, 15.32, 16.00, 16.72, 17.48, 18.28, 19.12, 20.00, 20.92, 21.88, 22.88, 23.92, 25.00, 26.12, 27.28, 28.48, 29.72, 31.00, 32.32, 33.68, 35.08, 36.52, 38.00, 39.52, 41.08, 42.68, 44.32, 46.00, 47.72, 49.48, 51.28, 53.12, 55.00, 56.92, 58.88, 60.88, 62.92, 65.00, 67.12, 69.28, 71.48, 73.72, 76.00, 78.32, 80.68, 83.08, 85.52, 88.00, 90.52, 93.08, 95.68, 98.32, 101.00, 103.72, 106.48, 109.28, 112.12, 115.00, 117.92, 120.88, 123.88, 126.92, 130.00, 133.12, 136.28, 139.48, 142.72, 146.00, 149.32, 152.68, 156.08, 159.52, 163.00};

#ifdef PRINT_TEST5
    std::cout << "trapezoidalRule test:" << std::endl;
    std::cout << trapezoidalRule(f, 0.1) << std::endl;
#endif

    return 0;
}
