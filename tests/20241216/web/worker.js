import loadWASM from '/dist/main.js';

// function fibonacci(n) {
//     if (n <= 1) {
//         return n;
//     }
//     return fibonacci(n - 1) + fibonacci(n - 2);
// }

onmessage = function (e) {
    console.log("Message received from main script");
    const { funcId, input } = e.data;
    const workerResult = `recieved: e.data=${e.data}`;
    const doCalc = (async () => {
        const Module = await loadWASM();
        console.log(Module);
        return await Module.calc();
    });
    const result = doCalc();
    console.log(`calc result=${result}`);
    // 計算が完了したら結果をメインスレッドに送信
    postMessage({ result: 12 });
};


// onmessage = async function (e) {
//     console.log("Message received from main script");
//     const { funcId, input } = e.data;

//     try {
//         const Module = await loadWASM();
//         console.log(Module);

//         const result = await Module.calc();
//         console.log(`calc result=${result}`);

//         // Promiseではなく、実際の結果値を送信
//         postMessage({ result });
//     } catch (error) {
//         console.error('Error in worker:', error);
//         postMessage({ error: error.toString() });
//     }
// };
