#include <future>
#include <iostream>

#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <emscripten.h>

int long_running_calculation(int input) {
    // 重い計算をここで実行
    int result = 0;
    for(int i = 0; i < input * 1000000; i++) {
        result += i;
    }
    return result;
}

int calc()
{
    std::future<int> fut = std::async(std::launch::async, [] { return 42; });
    return fut.get();
}

void async_calculation(int input, emscripten::val callback) {
    // auto f = [&] {
    //     std::future<int> fut = std::async(std::launch::async, long_running_calculation, input);
    //     fut.wait();
    //     int result = fut.get();
    //     callback(result);
    // };
    std::future<int> fut = std::async(std::launch::async, long_running_calculation, input);
    fut.wait();
    int result = fut.get();
    callback(result);
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("asyncCalculation", &async_calculation);
}

EMSCRIPTEN_KEEPALIVE
int main(int arg, char** argv){
    // print hello world
    std::cout << "Hello World!" << std::endl;
    std::cout << "This is a C++ program: argc=" << arg << std::endl;
    for (int i = 0; i < arg; i++) {
        std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
    }

  EM_ASM(
    console.log('hello world!, this is console.log from EM_ASM');
    Module.asyncCalculation(100).then(
        result => { console.log('Calculation result:', result); }) .catch(error => { console.error('Calculation error:', error); });
  );
  return 0;
}
