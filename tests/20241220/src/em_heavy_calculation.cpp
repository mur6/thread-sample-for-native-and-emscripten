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
    std::cout << "start_calculation: 計算開始: " << n << std::endl;
    // invoke heavy computation in detached thread
    std::thread([n] {
        // 重い計算を実行
        std::cout << "thread: 計算開始: " << n << std::endl;
        int result = heavy_computation(n);
        // 結果をコールバック
        // EM_ASM({
        //     // check Module has onCalcComplete function
        //     if (window.onCalcComplete) {
        //         window.onCalcComplete($0);
        //     } else {
        //         console.error('window.onCalcComplete is not defined');
        //     }
        // }, result);
        // JavaScript側のコールバック関数を呼び出す
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "計算結果: " << result << std::endl;
        auto jsCallback = emscripten::val::global("onCalcComplete");
        if (jsCallback.typeOf().as<std::string>() == "function") {
            jsCallback(result); // 計算結果を渡す
        } else {
            // emscripten::val::global("console").call<void>("error", "Callback is not registered.");
            // console.log("Callback is not registered.");
            std::cout << "Callback is not registered." << std::endl;
        }
    }).detach();
}


EMSCRIPTEN_BINDINGS(module) {
    emscripten::function("startCalculation", &start_calculation);
}
