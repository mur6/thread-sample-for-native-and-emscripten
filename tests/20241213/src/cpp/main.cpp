#include <queue>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <thread>
#include <array>
#include <algorithm>
#include <random>
#include <emscripten.h>

using namespace std::literals::chrono_literals;

int main()
{
    puts("Before the thread");

    // int arg = 42;
    std::vector<std::thread> threads;
    int thread_count = 20;
    for (int i = 0; i < thread_count; i++)
    {
        threads.push_back(std::thread([i]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Inside the thread: " << i << std::endl;
        }));
    }
    // std::thread thread([&]() {
    //     std::this_thread::sleep_for(std::chrono::seconds(1));
    //     std::cout << "Inside the thread: " << arg << std::endl;
    // });

    for (auto &thread : threads)
    {
        thread.join();
    }
    // destroy terminated web worker threads in emscripten.
    // for (auto &thread : threads)
    // {
    //     emscripten_clean_up_thread(thread.native_handle());
    // }

    std::cout << "After the thread" << std::endl;

    emscripten_exit_with_live_runtime(); // スレッドを使うときは、大事っぽい。
    return 0;
}
