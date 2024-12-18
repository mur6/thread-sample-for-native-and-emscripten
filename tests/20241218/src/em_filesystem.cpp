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
    // ここにメインループの処理を記述
    // この関数は毎フレーム呼び出される
    // この関数内で重い処理を行うと、ブラウザがフリーズするので注意
    // また、この関数内でエラーが発生すると、ループが停止するので注意
    // この関数内でエラーが発生しても、エラーがコンソールに表示されないので注意
    // エラーが発生した場合は、この関数内でデバッガを使ってエラーを解決する
    std::cout << "fps=1" << std::endl;
}

void main_loop_fps_2() {
    // ここにメインループの処理を記述
    // この関数は毎フレーム呼び出される
    // この関数内で重い処理を行うと、ブラウザがフリーズするので注意
    // また、この関数内でエラーが発生すると、ループが停止するので注意
    // この関数内でエラーが発生しても、エラーがコンソールに表示されないので注意
    // エラーが発生した場合は、この関数内でデバッガを使ってエラーを解決する
    std::cout << "fps=2" << std::endl;
}

int main() {
    // 初期化処理

    emscripten_set_main_loop(main_loop_fps_1, 1, 1);
    emscripten_set_main_loop(main_loop_fps_2, 2, 1);

    return 0;
}
