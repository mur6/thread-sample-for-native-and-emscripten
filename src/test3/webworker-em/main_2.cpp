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

EM_JS(EM_VAL, create_web_worker, (emscripten::EM_VAL callbackFunc), {
    var func = Emval.toValue(val_handle);
    var json = func.call<val>();
    return Emval.toHandle(json);
});

// JavaScript から呼び出す関数
int run_heavy_computation() {
    // thread を使って計算を行う
    std::promise<long long> promise;
    std::future<long long> future = promise.get_future();
    std::thread thread([&] {
        // get thread id
        auto tid = std::this_thread::get_id();
        auto worker_id = emscripten_wasm_worker_self_id();
        std::cout << "Thread ID: " << tid << std::endl;
        std::cout << "Worker ID: " << worker_id << std::endl;
        // 重い計算を行う
        auto r = heavy_computation(44);
        std::cout << "計算結果: " << r << std::endl;
    });
    thread.detach();
    // return worker id
    return 42;
}

std::string test() {
    auto testfunc = []() {
        return "Hello from worker!";
    };
    val obj = val::take_ownership(create_web_worker(testfunc));
    std::string result = obj.as<std::string>();
    // auto json = create_web_worker(testfunc);
    // std::string result = json.as<std::string>();
    return result;
}


// JavaScript にバインド
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("runHeavyComputation", &run_heavy_computation);
    emscripten::function("test", &test);
}

int main() {
    // メインスレッドでの処理
    std::cout << "Main thread: " << std::this_thread::get_id() << std::endl;
    // long livig in main thread
    emscripten_exit_with_live_runtime();
    return 0;
}
