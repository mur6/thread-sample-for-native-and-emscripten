console.log("javascripts/test02/entry.js loaded");

import loadWASM from '/dist/em_heavy_calculation.js';

let Module = null;

const onCalcComplete = (retValue) => {
    console.log(`onCalcComplete: retValue=${retValue}`);
};

const initModule = async () => {
    try{
        if (!Module) {
            Module = await loadWASM();
            console.log("Module created", Module);
        }
        Module["onCalcComplete"] = (retValue) => {
            console.log(`onCalcComplete: retValue=${retValue}`);
        }
    } catch (error) {
        console.error("Error loading or running WASM:", error); // Catch and log the error
    }
}

const main = async () => {
    await initModule();
    console.log("main() start");
    const result = Module.startCalculation(20);
    console.log("main() end", result);
}

main();

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
