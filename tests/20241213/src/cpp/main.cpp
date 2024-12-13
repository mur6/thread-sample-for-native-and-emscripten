#include <queue>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <thread>
#include <array>
#include <algorithm>
#include <random>

using namespace std::literals::chrono_literals;

int main()
{
    puts("Before the thread");

    int arg = 42;
    std::thread thread([&]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Inside the thread: " << arg << std::endl;
    });

    thread.join();

    std::cout << "After the thread" << std::endl;

    return 0;
}
