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
    for(int i = 0; i < 10; i++) {
        // get thread id
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
        std::cout << "計算中... " << i << std::endl;
    }
    return input * 2;
}

// emscripten::val wait(int ms) {
//     return emscripten::val::global("Promise").new_(
//         emscripten::val::module_property("function")([ms](emscripten::val resolve) {
//             emscripten::val::global("setTimeout").call<void>(
//                 emscripten::val::module_property("function")([resolve, ms]() {
//                     resolve(emscripten::val("Waited for " + std::to_string(ms) + " milliseconds"));
//                 }),
//                 ms
//             );
//         })
//     );
// }

emscripten::val getNumber() {
    // JavaScriptの Promise.resolve(42) を生成
    return emscripten::val::global("Promise").call<emscripten::val>("resolve", 42);
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
        // get thread id
        std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
        std::cout << "メインスレッド：他の処理を実行中..." << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        emscripten_sleep(1000);
    }
    return future.get();
}


int async_calculation2(int input) {
    return long_running_calculation(input);
}


EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("async_calculation", &async_calculation);
    emscripten::function("async_calculation2", &async_calculation2);
    emscripten::function("getNumber", &getNumber);
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
