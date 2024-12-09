#include <future>
#include <iostream>

#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <emscripten.h>

int calc()
{
    std::future<int> fut = std::async(std::launch::async, [] { return 42; });
    return fut.get();
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("calc", &calc);
}

EMSCRIPTEN_KEEPALIVE
int main(int arg, char** argv){
    // print hello world
    std::cout << "Hello World!" << std::endl;
    std::cout << "This is a C++ program: argc=" << arg << std::endl;
    for (int i = 0; i < arg; i++) {
        std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
    }

  EM_ASM(
    console.log('hello world!, this is console.log from EM_ASM');
    const r = Module.calc();
    console.log('Module.calc=', r);
  );
  return 0;
}
