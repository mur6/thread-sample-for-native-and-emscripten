# thread-sample-for-native-and-emscripten

C++を使用したスレッドのサンプルプログラム.

ネイティブ、またはemscriptenでのビルドをテストする目的で作成した.

## 使用法: ネイティブ版

```sh
 ❯ sh make.sh all
Finding primes between 3 and 10000000
Found 664578 primes
Time taken: 1.02393s
Finding primes between 3 and 10000000
Using 10 threads
Found 664578 primes
Time taken: 0.168812s
```

## 使用法: wasm版

### 必要な準備

下記のようにして emsdk を有効化すること.

```sh
export EMSDK_ENV_PATH=/path/to/emsdk
source $EMSDK_ENV_PATH/emsdk_env.sh
```

### ビルド

```sh
❯ sh make_em.sh build
❯ python3 server.py
```

http://localhost:8080/build/calculate_primes.html

にアクセスし、結果を確認する。


### ビルド(並列版)

```sh
❯ sh make_em.sh build_p
❯ python3 server.py
```

http://localhost:8080/build/calculate_primes_parallel.html

にアクセスし、結果を確認する。

