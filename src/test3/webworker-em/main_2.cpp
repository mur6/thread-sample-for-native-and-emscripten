#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/threading.h>
#include <emscripten/wasm_worker.h>
#include <future>
#include <iostream>


using namespace emscripten;

// 計算が重い処理（例: フィボナッチ数を計算）
int heavy_computation(int n) {
    if (n <= 1) return n;
    return heavy_computation(n - 1) + heavy_computation(n - 2);
}


std::string test() {
    return "test test";
}

// state machine,  that will be called by the worker
void run_heavy_computation(int n, std::function<void(int)> callback) {
    // 重い計算を実行
    int result = heavy_computation(n);
    // 結果をコールバック
    callback(result);
}


// JavaScript にバインド
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("runHeavyComputation", &run_heavy_computation);
    emscripten::function("test", &test);
}
