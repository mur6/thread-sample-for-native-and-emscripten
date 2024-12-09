#include <future>
#include <iostream>

#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <emscripten.h>

int long_running_calculation(int input) {
    // 重い計算をここで実行
    // int result = 0;
    // for(int i = 0; i < input * 100; i++) {
    //     result += i;
    // }
    // std::this_thread::sleep_for(std::chrono::seconds(20));
    for(int i = 0; i < 30; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "計算中... " << i << std::endl;
    }
    return input * 2;
}

int calc()
{
    std::future<int> fut = std::async(std::launch::async, [] { return 42; });
    return fut.get();
}

int async_calculation(int input) {
    std::promise<int> promise;
    std::future<int> future = promise.get_future();


    std::thread thread([&] {
        int result = long_running_calculation(input);
        promise.set_value(result);
    });
    thread.detach();
    std::cout << "メインスレッド：他の処理を実行中..." << std::endl;
    // check future is done
    while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        std::cout << "メインスレッド：他の処理を実行中..." << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        emscripten_sleep(1000);
    }
    return future.get();
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("asyncCalculation", &async_calculation);
}

// EMSCRIPTEN_KEEPALIVE
// int main(int arg, char** argv){
//     // print hello world
//     std::cout << "Hello World!" << std::endl;
//     std::cout << "This is a C++ program: argc=" << arg << std::endl;
//     for (int i = 0; i < arg; i++) {
//         std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
//     }

//   EM_ASM(
//     console.log('hello world!, this is console.log from EM_ASM');
//     (async () => {
//     const result = await Module.asyncCalculation(42);
//     console.log('result:', result);
//     })();
//     setTimeout(() => {
//         console.log('setTimeout');
//     }, 1000);
//     setInterval(() => {
//         console.log('setInterval');
//     }, 1000);
//   );

//   emscripten_exit_with_live_runtime();
//   return 0;
// }
