// Build: g++ test_mv_semantics_and_cp_elision.cpp -Wall -Wpedantic -o test_mv_semantics_and_cp_elision.out
// Execute: ./test_mv_semantics_and_cp_elision.out

#include <iostream>
#include <utility>

class Random
{
    public:
        Random() {std::cout << "Constructed" << std::endl;}
        Random(const Random& random) {std::cout << "Copied" << std::endl;}
        Random(Random&& random) {std::cout << "Moved" << std::endl;}
        ~Random() {std::cout << "Destructed" << std::endl;}
};

struct Arbitrary
{
    Random random;
};

int main()
{
    // // Result: Constructed > Copied > Destructed > Destructed
    // Random random1;
    // Random random2 {random1};

    // // Result: Constructed > Moved > Destructed > Destructed (move semantics)
    // Random random1;
    // Random random2 {std::move(random1)};

    // // Result: Constructed > Destructed (copy elision)
    // Random random {Random()};

    // Result: Constructed > Destructed (also copy elision)
    Arbitrary arbitrary {Random()};

    return 0;
}
