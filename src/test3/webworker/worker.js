function fibonacci(n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

onmessage = function (e) {
    const { funcId, input } = e.data;

    console.log("Message received from main script");
    const workerResult = `Result: ${e.data[0] * e.data[1]}`;
    console.log("Posting message back to main script");
    const result = fibonacci(input);
    // 計算が完了したら結果をメインスレッドに送信
    postMessage({ result });
};
