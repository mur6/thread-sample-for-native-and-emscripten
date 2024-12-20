#include <emscripten.h>
#include <emscripten/bind.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>

// std::atomic<bool> calculation_complete(false);
// std::atomic<int> result(0);

// // メモリのオフセット位置を取得
// extern "C" {
//     int get_calculation_complete_address() {
//         return reinterpret_cast<int>(&calculation_complete);
//     }
// }

// void heavy_calculation_by_using_atomic() {
//     calculation_complete.store(false);
//     std::this_thread::sleep_for(std::chrono::seconds(7));
//     result.store(42); // 計算結果をセット
//     calculation_complete.store(true);
// }

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
    // invoke heavy computation in detached thread
    std::thread([n] {
        // 重い計算を実行
        int result = heavy_computation(n);
        // 結果をコールバック
        EM_ASM({
            // check Module has onCalcComplete function
            if (Module.onCalcComplete) {
                Module.onCalcComplete($0);
            } else {
                console.error('onCalcComplete is not defined');
            }
        }, result);
    }).detach();
}


EMSCRIPTEN_BINDINGS(module) {
    emscripten::function("startCalculation", &start_calculation);
}