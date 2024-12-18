#include <emscripten.h>
#include <emscripten/bind.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>

std::atomic<bool> calculation_complete(false);
std::atomic<int> result(0);

// メモリのオフセット位置を取得
extern "C" {
    int get_calculation_complete_address() {
        return reinterpret_cast<int>(&calculation_complete);
    }
}

void heavy_calculation() {
    calculation_complete.store(false);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    result.store(42);  // 計算結果をセット
    calculation_complete.store(true);
}

void main_loop() {
    if (calculation_complete.load()) {
        printf("Calculation completed. Result: %d\n", result.load());
        emscripten_cancel_main_loop();
    } else {
        auto now = std::chrono::system_clock::now();
        std::cout << "Main loop running at " << std::chrono::system_clock::to_time_t(now) << std::endl;
    }
}

void start_calculation() {
    calculation_complete.store(false);
    std::thread(heavy_calculation).detach();
    // emscripten_set_main_loop(main_loop, 5, 1);
}

int get_calculation_result() {
    return result.load();
}

EMSCRIPTEN_BINDINGS(module) {
    emscripten::function("startCalculation", &start_calculation);
    emscripten::function("getCalculationResult", &get_calculation_result);
    emscripten::function("get_calculation_complete_address", &get_calculation_complete_address);
}

// int main() {
//     EM_ASM(
//         console.log('before startCalculation');
//         Module.startCalculation();
//         console.log('after startCalculation');
//         const address = Module.get_calculation_complete_address(); // アドレスを取得
//         const HEAP8 = new Int8Array(Module.HEAP8.buffer);
//         // const interval = setInterval(() => {
//         //     const value = Atomics.load(HEAP8, address);
//         //     statusElement.textContent = `calculation_complete Status: ${value ? "Done" : "In Progress"}`;
//         //     if (value) {
//         //         console.log('Calculation result:', Module.getCalculationResult());
//         //         clearInterval(interval);
//         //     }
//         // }, 1000);
//         const interval = setInterval(() => {
//             console.log('interval');
//         }, 1000);
//     );
// }
