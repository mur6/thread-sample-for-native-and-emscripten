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

std::mutex mtx;

void create_and_join_threads(int try_index, int thread_num) {
    std::vector<std::thread> threads;
    for (int i = 0; i < thread_num; i++)
    {
        threads.push_back(std::thread([&, i]() {
            std::this_thread::sleep_for(2ms);
            {
                std::lock_guard<std::mutex> lock(mtx);
                std::cout << "try_index=" << try_index << " thread_num=" << thread_num << " id=" << i << std::endl;
            }
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
}


int main()
{
    puts("Before the thread");

    // int arg = 42;
    std::vector<int> thread_nums = {5, 9, 10, 8, 9, 11, 10};
    int thread_try_count = thread_nums.size();
    for (int i = 0; i < thread_try_count; i++)
    {
        create_and_join_threads(i, thread_nums[i]);
    }
    std::cout << "After the thread" << std::endl;

    emscripten_exit_with_live_runtime(); // スレッドを使うときは、大事っぽい。
    return 0;
}
