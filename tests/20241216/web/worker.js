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
    const { funcName, input } = e.data;
    console.log(`recieved: funcName=${funcName}`);
    console.log(`recieved: e.data=${e.data}`);

    if (!Module) {
        Module = await loadWASM();
    }

    if (funcName === "appendAndSumOfAll") {
        const inputNums = e.data;
        console.log(`inputNums=${inputNums}`);
        const vectorInt = await Module.convertJSArrayToVector(inputNums);
        console.log(`vectorInt=${vectorInt}`);
        const result = Module.appendAndSumOfAll(vectorInt);
        postMessage({ result: result });
    } else if (funcName === "heavyCalc") {
        const result = await Module.heavyCalc();
        postMessage({ result: result });
    } else {
        postMessage({ error: `unknown funcName=${funcName}` });
    }
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
