// Build: g++ test_math_in_loop.cpp -Wall -Wextra -Wpedantic -o test_math_in_loop.out
// Execute: time ./test_math_in_loop.out

int main()
{
    const int kLen {1000};
    int a {0};
    for (int i {0}; i < 1000000; i++)
    {
        for (int j {0}; j < kLen + 1; j++)
            a += j;
    }

    return 0;
}
