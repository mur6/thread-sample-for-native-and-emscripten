#include <queue>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <thread>
#include <array>
#include <algorithm>
#include <random>

using namespace std::literals::chrono_literals;

class mt_queue
{
    static const int capacity = 10;
    std::queue<int> q_;
    std::mutex mtx_;
    std::condition_variable cv_; // ★条件変数を追加

public:
    void push(int data)
    {
        std::unique_lock<std::mutex> lk(mtx_);
        while (q_.size() == capacity)
        {
            cv_.wait(lk); // 要素数変更があるまで待機
        }
        q_.push(data);
        cv_.notify_all(); // 要素数変更を通知
    }
    int pop()
    {
        std::unique_lock<std::mutex> lk(mtx_);
        while (q_.empty())
        {
            cv_.wait(lk); // 要素数変更があるまで待機
        }
        int data = q_.front();
        q_.pop();
        cv_.notify_all();  // 要素数変更を通知
        return data;
    }
};

int main()
{
    std::array<int, 10> ar;
    std::iota(ar.begin(), ar.end(), 0); // initialize array with 0, 1, 2, ..., 9999
    std::shuffle(ar.begin(), ar.end(), std::mt19937{std::random_device{}()});
    // print array. line feed after 50 elements
    // for (int i = 0; i < 500; ++i)
    // {
    //     std::cout << ar[i] << " ";
    //     if ((i + 1) % 50 == 0)
    //     {
    //         std::cout << std::endl;
    //     }
    // }
    for (int i : ar)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    mt_queue q;

    std::thread t1([&q, &ar] {
        for (int i : ar) {
            q.push(i);
            // sleep
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });
    std::thread t2([&q] {
        for (int i = 0; i < 10; ++i) {
            int data = q.pop();
            std::cout << data << std::endl;
        }
    });
    t1.join();
    t2.join();
    return 0;
}
