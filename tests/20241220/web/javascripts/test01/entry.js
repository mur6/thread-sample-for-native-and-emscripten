console.log("test01/entry.js loaded");

const worker = new Worker("./worker.js");

const run = async () => {
    try{
        const Module = await loadWASM();
        console.log("Module.startCalculation(): start");
        const result = await calcAsync(Module);
        console.log(`Module.startCalculation(): end, result=${result}`);
        document.getElementById("output-result").innerText = result;

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
