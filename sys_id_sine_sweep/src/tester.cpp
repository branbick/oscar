// Build: g++ tester.cpp sys_id_sine_sweep.cpp -Wall -Wextra -Wpedantic -std=c++17 -o tester.out
// Execute: ./tester.out (> tester.txt)

#include "sys_id_sine_sweep.hpp"

#include <iostream>

int main()
{
    // // Generate and print a logarithmically spaced vector
    // const std::vector<double> kLogSpacedVec {logSpace(10.0, 1000.0, 5)};
    // for (const double kEl : kLogSpacedVec)
    //     std::cout << kEl << " ";
    // std::cout << std::endl;

    // Generate and print an input signal
    const std::vector<double> kInputSignal {generateInputSignal(1.0,
                                                                10.0,
                                                                500.0,
                                                                5,
                                                                3,
                                                                500)};
    for (const double kEl : kInputSignal)
        std::cout << kEl << " ";
    std::cout << std::endl;

    return 0;
}
