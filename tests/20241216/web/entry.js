var worker = new Worker("worker.js", { type: 'module' });

function _sendToWorker(funcName, input) {
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

async function sendToWorker(funcName, input) {
    console.log("sendToWorker: start");
    const result = await _sendToWorker(funcName, input);
    console.log("sendToWorker: end", result);
    return result;
}

// bind event to button, and write calc result to output div.
document.getElementById("store-nums").addEventListener("click", async () => {

    document.getElementById("output").textContent = result;
});

document.getElementById("term-worker").addEventListener("click", async () => {
    worker.terminate();
    worker = null;
    console.log("worker terminated");
});

document.getElementById("recreate-worker").addEventListener("click", async () => {
    worker = new Worker("worker.js", { type: 'module' });
    console.log("worker recreated");
});
/* <button id="store-nums">数列をストアする</button>
<button id="heavy-calc">ストアされた数列の合計値で、フィボナッチ数を計算する</button>
<button id="term-worker">workerを削除</button>
<button id="recreate-worker">workerを再生成</button> */
