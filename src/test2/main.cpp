#include <future>

int main()
{
    std::future<int> fut = std::async(std::launch::async, [] { return 42; });
    return fut.get();
}
