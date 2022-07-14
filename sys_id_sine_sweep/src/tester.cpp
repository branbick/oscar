// Build: g++ tester.cpp sys_id_sine_sweep.cpp -Wall -Wextra -Wpedantic -std=c++17 -o tester.out
// Execute: ./tester.out (> tester.txt)

#include "sys_id_sine_sweep.hpp"

#include <iostream>

int main()
{
    // // Generate and print a logarithmically spaced vector
    // const std::vector<double> kLogSpacedVec {logSpace(10.0, 1000.0, 7)};
    // for (const double kEl : kLogSpacedVec)
    //     std::cout << kEl << " ";
    // std::cout << std::endl;

    // // Generate and print an input signal
    // const std::vector<double> kInputSignal {generateInputSignal(1.0,
    //                                                             10.0,
    //                                                             1000.0,
    //                                                             10,
    //                                                             10,
    //                                                             1000.0)};
    // for (const double kEl : kInputSignal)
    //     std::cout << kEl << " ";
    // std::cout << std::endl;

    // Numerically compute the integral of the function below using the
    // (composite) trapezoidal rule
    //
    // f(t) = 2*t^2 - 5*t + 13 for t = [0.0, 10.0] s and T = 0.1 s
    //
    // MATLAB code:
    // t = [0.0:0.1:10.0];  % (s)
    // f = 2*t.^2 - 5*t + 13;
    // for i = 1:length(t), fprintf('%.2f, ', f(i)), end, fprintf('\n')
    // trapz(t,f)
    std::vector<double> f {13.00, 12.52, 12.08, 11.68, 11.32, 11.00, 10.72, 10.48, 10.28, 10.12, 10.00, 9.92, 9.88, 9.88, 9.92, 10.00, 10.12, 10.28, 10.48, 10.72, 11.00, 11.32, 11.68, 12.08, 12.52, 13.00, 13.52, 14.08, 14.68, 15.32, 16.00, 16.72, 17.48, 18.28, 19.12, 20.00, 20.92, 21.88, 22.88, 23.92, 25.00, 26.12, 27.28, 28.48, 29.72, 31.00, 32.32, 33.68, 35.08, 36.52, 38.00, 39.52, 41.08, 42.68, 44.32, 46.00, 47.72, 49.48, 51.28, 53.12, 55.00, 56.92, 58.88, 60.88, 62.92, 65.00, 67.12, 69.28, 71.48, 73.72, 76.00, 78.32, 80.68, 83.08, 85.52, 88.00, 90.52, 93.08, 95.68, 98.32, 101.00, 103.72, 106.48, 109.28, 112.12, 115.00, 117.92, 120.88, 123.88, 126.92, 130.00, 133.12, 136.28, 139.48, 142.72, 146.00, 149.32, 152.68, 156.08, 159.52, 163.00};
    std::cout << "integral: " << trapezoidalRule(f, 0.1) << std::endl;

    return 0;
}
