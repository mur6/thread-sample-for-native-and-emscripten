console.log("javascripts/test02/entry.js loaded");

import loadWASM from '/dist/main.js';

let Module = null;

const run = async () => {
    try{
        if (!Module) {
            Module = await loadWASM();
            console.log("Module created", Module);
        }
    } catch (error) {
        console.error("Error loading or running WASM:", error); // Catch and log the error
    }
}

run();

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
