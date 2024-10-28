#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

#include "const.hpp"

int IsPrime(int value)
{
    if (value == 2)
    {
        return 1;
    }
    if (value == 1 || value % 2 == 0)
    {
        return 0;
    }
    for (int i = 3; i <= sqrt(value); i += 2)
    {
        if (value % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

void FindPrimes(int start, int end, std::vector<int> &primes_found)
{
    // 偶数が入力された場合はインクリメントして奇数にする
    if (start % 2 == 0)
    {
        start++;
    }

    for (int i = start; i <= end; i++)
    {
        if (IsPrime(i))
        {
            primes_found.push_back(i);
        }
    }
}

int main()
{
    auto start_time = std::chrono::system_clock::now();

    int start = 3, end = primeNumberUpperBound;
    std::cout << "Finding primes between " << start << " and " << end << std::endl;
    std::vector<int> primes_found;
    FindPrimes(start, end, primes_found);
    // for (int i : primes_found)
    // {
    //     std::cout << i << std::endl;
    // }
    std::cout << "Found " << primes_found.size() << " primes" << std::endl;
    auto end_time = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    std::cout << "Time taken: " << elapsed_seconds.count() << "s" << std::endl;
    return 0;
}
