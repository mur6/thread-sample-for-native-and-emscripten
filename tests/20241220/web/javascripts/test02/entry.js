console.log("javascripts/test02/entry.js loaded");

import loadWASM from '/dist/em_heavy_calculation.js';

let Module = null;
let calcResult = 0;

const onCalcComplete = (retValue) => {
    console.log(`onCalcComplete: retValue=${retValue}`);
    calcResult = retValue;
};

const initModule = async () => {
    try{
        if (!Module) {
            Module = await loadWASM();
            console.log("Module created", Module);
        }
    } catch (error) {
        console.error("Error loading or running WASM:", error); // Catch and log the error
    }
}

initModule();

async function calc_fib(n) {
    await initModule();
    Module.startCalculation(n);
    return new Promise((resolve, reject) => {
        const interval = setInterval(() => {
            if (calcResult !== 0) {
                const result = calcResult;
                clearInterval(interval);
                resolve(result);
            }
        }, 100);
    });
}

const registerEvents = () => {
    // <div>
    //     <h2>フィボナッチ数列を計算する</h2>
    //     <input type="number" id="fib-input-num" value="10" />
    //     <button id="fib-calc">計算</button>
    // </div>
    document.getElementById("fib-calc").addEventListener("click", async () => {
        const num_str = document.getElementById("fib-input-num").value;
        const num = parseInt(num_str, 10);
        console.log(`fib-calc: num=${num}`);
        const result = await calc_fib(num);
        console.log(`fib-calc: result=${result}`);
        document.getElementById("output-fib-result").innerText = result;
    });

    // ダミーの処理: カウンター
    let counter = 0;
    document.getElementById("increment").addEventListener("click", () => {
        // <p id="output-counter">0</p>
        counter++;
        document.getElementById("output-counter").innerText = counter;
    });
    document.getElementById("decrement").addEventListener("click", () => {
        // <p id="output-counter">0</p>
        counter--;
        document.getElementById("output-counter").innerText = counter;
    });

}

registerEvents();
