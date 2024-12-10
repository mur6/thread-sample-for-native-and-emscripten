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

// スレッドで実行されるタスク
void thread_task(void* arg) {
    auto* promise_data = reinterpret_cast<std::promise<int>*>(arg);
    int result = heavy_computation(40); // 計算の例 (フィボナッチ数)
    promise_data->set_value(result);    // 結果を Promise に設定
    delete promise_data;                // メモリを解放
}

// JavaScript から呼び出す関数
emscripten::val run_heavy_computation() {
    // Promise を作成
    auto* promise_data = new std::promise<int>();
    std::future<int> result = promise_data->get_future();

    // スレッドにタスクを非同期でディスパッチ
    emscripten_dispatch_to_thread_async(
        emscripten_main_runtime_thread_id(), // スレッド ID
        thread_task,                         // 実行する関数
        nullptr,                             // 完了コールバック（不要）
        reinterpret_cast<void*>(promise_data)
    );

    // JavaScript の Promise に変換して返す
    return emscripten::val::global("Promise").new_(
        [promise_data = std::move(result)](emscripten::val resolve, emscripten::val reject) mutable {
            try {
                resolve(promise_data.get());
            } catch (...) {
                reject(emscripten::val("Error in thread task"));
            }
        });
}

// JavaScript にバインド
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("runHeavyComputation", &run_heavy_computation);
}
