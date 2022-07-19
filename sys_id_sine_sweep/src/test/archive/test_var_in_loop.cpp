// Build: g++ test_var_in_loop.cpp -Wall -Wextra -Wpedantic -o test_var_in_loop.out
// Execute: time ./test_var_in_loop.out

int main()
{
    const int kLen {1000};
    int a {0};
    for (int i {0}; i < 1000000; i++)
    {
        const int kLenPlus1 {kLen + 1};
        for (int j {0}; j < kLenPlus1; j++)
            a += j;
    }

    return 0;
}
