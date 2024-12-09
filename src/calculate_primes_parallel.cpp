// calculate primes up to a given number

#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <chrono>

#include <emscripten.h>
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

std::mutex primes_mutex;

void FindPrimesParallel(int start, int end, std::vector<int> &primes_found)
{
    std::vector<int> local_primes;

    // 偶数が入力された場合はインクリメントして奇数にする
    if (start % 2 == 0)
    {
        start++;
    }

    for (int i = start; i <= end; i += 2)
    {
        if (IsPrime(i))
        {
            local_primes.push_back(i);
        }
    }

    // スレッドセーフな方法でメインの結果ベクターに結合
    std::lock_guard<std::mutex> lock(primes_mutex);
    primes_found.insert(primes_found.end(), local_primes.begin(), local_primes.end());
}

int main()
{
    auto start_time = std::chrono::system_clock::now();

    int start = 3, end = primeNumberUpperBound;
    std::cout << "Finding primes between " << start << " and " << end << std::endl;

    #ifdef __EMSCRIPTEN__
        unsigned int thread_count = 4;
    #else
        unsigned int thread_count = std::thread::hardware_concurrency();
        if (thread_count == 0)
            thread_count = 4;
    #endif
    std::cout << "Using " << thread_count << " threads" << std::endl;

    std::vector<std::thread> threads;
    std::vector<int> primes_found;

    // 2は別で処理（偶数のため）
    if (start <= 2)
        primes_found.push_back(2);

    // 範囲を分割して各スレッドに割り当て
    int range_per_thread = (end - start) / thread_count;

    for (unsigned int i = 0; i < thread_count; ++i)
    {
        int thread_start = start + (i * range_per_thread);
        int thread_end = (i == thread_count - 1) ? end : thread_start + range_per_thread - 1;

        threads.emplace_back(FindPrimesParallel, thread_start, thread_end, std::ref(primes_found));
        emscripten_sleep(0);
    }

    // 全スレッドの完了を待つ
    for (auto &thread : threads)
    {
        thread.join();
    }

    // 結果を昇順にソート
    std::sort(primes_found.begin(), primes_found.end());

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
