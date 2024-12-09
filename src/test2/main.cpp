#include <future>
#include <iostream>

int main()
{
    std::future<int> fut = std::async(std::launch::async, [] { return 42; });
    auto result = fut.get();
    std::cout << "The answer is " << result << std::endl;
    return 0;
}
