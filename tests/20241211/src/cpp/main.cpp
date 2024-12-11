#include <queue>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <thread>

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
    mt_queue q;
    // push
    q.push(123);
    // pop
    int data = q.pop();
    std::cout << data << std::endl;

    std::thread t1([&q] {
        q.push(1);
        q.push(2);
        q.push(3);
    });
    std::thread t2([&q] {
        std::cout << q.pop() << std::endl;
        std::cout << q.pop() << std::endl;
        std::cout << q.pop() << std::endl;
    });
    t1.join();
    t2.join();
    return 0;
}
