#include <emscripten.h>
#include <emscripten/bind.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>

std::atomic<bool> calculation_complete(false);
std::atomic<int> result(0);

// // メモリのオフセット位置を取得
// extern "C" {
//     int get_calculation_complete_address() {
//         return reinterpret_cast<int>(&calculation_complete);
//     }
// }

void heavy_calculation_wrapped() {
    calculation_complete.store(false);
    int result = heavy_computation(n);
    result.store(result);
    calculation_complete.store(true);
}

// void start_calculation() {
//     calculation_complete.store(false);
//     std::thread(heavy_calculation).detach();
// }

// int get_calculation_result() {
//     return result.load();
// }

// 計算が重い処理（例: フィボナッチ数を計算）
int heavy_computation(int n) {
    if (n <= 1) return n;
    return heavy_computation(n - 1) + heavy_computation(n - 2);
}

void start_calculation(int n) {
    std::cout << "start_calculation: 計算開始: " << n << std::endl;
    // invoke heavy computation in detached thread
    std::thread([n] {
        std::cout << "thread: 計算開始: " << n << std::endl;
        
        std::cout << "thread: 計算終了: " << result << std::endl;

    }).detach();
}


EMSCRIPTEN_BINDINGS(module) {
    emscripten::function("startCalculation", &start_calculation);
}
