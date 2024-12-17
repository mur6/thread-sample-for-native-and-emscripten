const worker = new Worker("worker.js", { type: 'module' });

function calculateInWorker(funcId, input) {
    return new Promise((resolve, reject) => {

        // Workerが結果を送信したときにPromiseを解決
        worker.onmessage = (e) => {
            resolve(e.data.result);
            // worker.terminate(); // Workerを終了
        };

        // Workerでエラーが発生したときにPromiseを拒否
        worker.onerror = (e) => {
            reject(new Error(`Worker error: ${e.message}`));
            // worker.terminate();
        };

        // Workerにデータを送信
        worker.postMessage({ funcId, input });
    });
}

// const run = () => {
//     setTimeout(async () => {
//         // const Module = await loadWASM();
//         console.log("calculateInWorker: start");
//         const result = await calculateInWorker(1, 10);
//         console.log("calculateInWorker: end", result);
//     }, 1000);
// };

// run();

// bind event to button, and write calc result to output div.
document.getElementById("btn").addEventListener("click", async () => {
    console.log("calculateInWorker: start");
    const result = await calculateInWorker(1, 10);
    console.log("calculateInWorker: end", result);
    document.getElementById("output").textContent = result;
});

document.getElementById("term-worker").addEventListener("click", async () => {
    worker.terminate();
    console.log("worker terminated");
});
