// Build: g++ tester.cpp sys_id_sine_sweep.cpp -Wall -Wextra -Wpedantic -std=c++17 -o tester.out
// Execute: ./tester.out

#include "sys_id_sine_sweep.hpp"

#include <iostream>

int main()
{
    std::vector<double> vec {logSpace(10.0, 1000.0, 5)};
    for (double el : vec)
        std::cout << el << " ";
    std::cout << std::endl;

    return 0;
}
