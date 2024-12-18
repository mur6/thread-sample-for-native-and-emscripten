#include <mutex>
#include <thread>
#include <iostream>
#include <emscripten.h>

struct MyStruct
{
    MyStruct(int num) : num(num) {}

    int getNum() const
    {
        return num;
    }

    void incrementNum()
    {
        std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);
        num++;
    }

private:
    int num;
};

int main2()
{
    MyStruct myStruct(42);
    std::thread t1([&myStruct]() {
        myStruct.incrementNum();
    });
    t1.detach();
    myStruct.incrementNum();

    EM_ASM({
        console.log('The number is ' + $0);
    }, myStruct.getNum());
    return 0;
}
void main_loop_fps_1() {
    std::cout << "fps=1" << std::endl;
    emscripten_async_call([](void*){
        main_loop_fps_1();
    }, nullptr, 1000); // 1秒ごとに呼び出し
}

void main_loop_fps_2() {
    std::cout << "fps=2" << std::endl;
    emscripten_async_call([](void*){
        main_loop_fps_2();
    }, nullptr, 500); // 0.5秒ごとに呼び出し
}

int main() {
    main_loop_fps_1();
    main_loop_fps_2();
    emscripten_exit_with_live_runtime(); // ランタイムを継続させる
    return 0;
}
