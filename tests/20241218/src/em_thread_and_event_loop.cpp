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
    std::this_thread::sleep_for(std::chrono::seconds(7));
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
    emscripten_set_main_loop(main_loop, 5, 1);
}

int get_calculation_result() {
    return result.load();
}

EMSCRIPTEN_BINDINGS(module) {
    emscripten::function("startCalculation", &start_calculation);
    emscripten::function("getCalculationResult", &get_calculation_result);
}

int main() {
    EM_ASM(
        console.log('WASM module loaded');
        Module.startCalculation();
        const interval = setInterval(() => {
            const result = Module.getCalculationResult();
            if (result !== 0) {
                console.log('Calculation result:', result);
                clearInterval(interval);
            }
        }, 1000);
    );
}
