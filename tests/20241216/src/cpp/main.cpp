#include <queue>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <thread>
#include <array>
#include <algorithm>
#include <random>
#include <emscripten.h>
#include <emscripten/bind.h>

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

static int answer_s = 0;

int calc()
{
    std::vector<int> thread_nums = {1, 2, 3, 4, 5};
    int thread_try_count = thread_nums.size();
    for (int i = 0; i < thread_try_count; i++)
    {
        create_and_join_threads(i, thread_nums[i]);
    }
    answer_s++;
    return answer_s;
}

// int main()
// {
//     puts("Before the thread");
//     std::vector<int> thread_nums = {1, 2, 3, 4, 5};
//     int answer = calc(thread_nums);
//     std::cout << "After the thread" << std::endl;
//     emscripten_exit_with_live_runtime(); // スレッドを使うときは、大事っぽい。
//     return 0;
// }

EMSCRIPTEN_BINDINGS(my_module)
{
    emscripten::register_vector<int>("VectorInt");
    emscripten::function("calc", &calc);
}
