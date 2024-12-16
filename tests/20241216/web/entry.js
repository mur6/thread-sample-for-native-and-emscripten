import loadWASM from '/dist/main.js';

function calculateInWorker(funcId, input) {
    return new Promise((resolve, reject) => {
        const worker = new Worker("worker.js");

        // Workerが結果を送信したときにPromiseを解決
        worker.onmessage = (e) => {
            resolve(e.data.result);
            worker.terminate(); // Workerを終了
        };

        // Workerでエラーが発生したときにPromiseを拒否
        worker.onerror = (e) => {
            reject(new Error(`Worker error: ${e.message}`));
            worker.terminate();
        };

        // Workerにデータを送信
        worker.postMessage({ funcId, input });
    });
}

const run = () => {
    setTimeout(async () => {
        // const Module = await loadWASM();
        console.log("calculateInWorker: start");
        const result = await calculateInWorker(1, 10);
        console.log("calculateInWorker: end", result);
    }, 1000);
};

run();