#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/threading.h>
#include <future>
#include <iostream>

// 計算が重い処理（例: フィボナッチ数を計算）
int heavy_computation(int n) {
    if (n <= 1) return n;
    return heavy_computation(n - 1) + heavy_computation(n - 2);
}

// JavaScript から呼び出す関数
emscripten::val run_heavy_computation() {
    // thread を使って計算を行う
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    std::thread thread([&] {
        // get thread id
        auto worker_id = emscripten_wasm_worker_self_id();
        std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
        std::cout << "Worker ID: " << worker_id << std::endl;
        int result = heavy_computation(40);
        promise.set_value(result);
    });
    thread.detach();
    // JavaScript の Promise に変換して返す
    return emscripten::val::global("Promise").new_(
        [&](emscripten::val resolve, emscripten::val reject) mutable {
            try {
                // while(future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
                //     emscripten_sleep(1000);
                // }
                resolve(42);
            } catch (...) {
                reject(emscripten::val("Error"));
            }
        }
    );
}

// JavaScript にバインド
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("runHeavyComputation", &run_heavy_computation);
}
