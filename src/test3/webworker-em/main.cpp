#include <iostream>
#include <future>


// web worker emscripten

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <emscripten/promise.h>

int fibb(int n) {
    if (n <= 1) {
        return n;
    }
    return fibb(n - 1) + fibb(n - 2);
}

std::promise<int> calc_fibbonacci_helper(int n) {
    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    std::thread thread([promise = std::move(promise), n]() mutable {
        int result = fibb(n);
        promise.set_value(result);
    });
    thread.detach();
    return promise;
}

emscripten::val calc_fibbonacci(int n) {
    auto result  = co_await calc_fibbonacci_helper(n);
    return emscripten::val(result);
}

EMSCRIPTEN_BINDINGS(module) {
    emscripten::function("calc_fibbonacci", &calc_fibbonacci);
}
