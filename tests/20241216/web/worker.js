import loadWASM from '/dist/main.js';

// function fibonacci(n) {
//     if (n <= 1) {
//         return n;
//     }
//     return fibonacci(n - 1) + fibonacci(n - 2);
// }
let Module = null;


onmessage = async function (e) {
    console.log("Message received from main script");
    const { funcId, input } = e.data;
    const workerResult = `recieved: e.data=${e.data}`;
    //const doCalc = (async () => {
    if (!Module) {
        Module = await loadWASM();
    }
    console.log(Module);
    const inputNums = [1, 2, 3, 4, 5];
    console.log(`inputNums=${inputNums}`);
    const vectorInt = await Module.convertJSArrayToVector(inputNums);
    console.log(`vectorInt=${vectorInt}`);
    const result = Module.appendAndSumOfAll(vectorInt);
    postMessage({ result: result });
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
