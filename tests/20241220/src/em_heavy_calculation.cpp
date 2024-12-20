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

// 計算が重い処理（例: フィボナッチ数を計算）
int heavy_computation(int n) {
    if (n <= 1) return n;
    return heavy_computation(n - 1) + heavy_computation(n - 2);
}

void heavy_calculation_wrapped(int n) {
    calculation_complete.store(false);
    int r = heavy_computation(n);
    result.store(r);
    calculation_complete.store(true);
}

// void start_calculation() {
//     calculation_complete.store(false);
//     std::thread(heavy_calculation).detach();
// }

// int get_calculation_result() {
//     return result.load();
// }

void check_status_and_call_js_callback() {
    if (calculation_complete.load()) {
        std::cout << "check_status_and_call_js_callback: 計算終了." << std::endl;
        int r = result.load();
        EM_ASM({
            // check Module has onCalcComplete function
            if (window.onCalcComplete) {
                window.onCalcComplete($0);
            } else {
                console.error('window.onCalcComplete is not defined');
            }
        }, r);
        calculation_complete.store(false);
        // cancel this loop
        return;
    }
    std::cout << "check_status_and_call_js_callback: 計算中..." << std::endl;
    emscripten_async_call([](void*){
        check_status_and_call_js_callback();
    }, nullptr, 500); // 0.5秒ごとに呼び出し
}

void start_calculation(int n) {
    std::cout << "start_calculation: 計算開始: " << n << std::endl;
    // invoke heavy computation in detached thread
    std::thread([n] {
        std::cout << "thread: 計算開始: " << n << std::endl;
        heavy_calculation_wrapped(n);
        std::cout << "thread: 計算終了." << std::endl;
    }).detach();
    check_status_and_call_js_callback();
}

EMSCRIPTEN_BINDINGS(module) {
    emscripten::function("startCalculation", &start_calculation);
}
