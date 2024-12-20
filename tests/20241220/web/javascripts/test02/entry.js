console.log("javascripts/test02/entry.js loaded");


// const run = () => {
//     worker.onmessage = (e) => {
//         const { result } = e.data;
//         console.log(`message recieved: result=${result}`);
//     }
//     console.log("worker created", worker);
//     try{
//         console.log("run() start");
//         worker.postMessage({ funcName: "add", input: { a: 1, b: 2 } });
//         console.log("message sent");
//     } catch (error) {
//         console.error("Error loading or running WASM:", error); // Catch and log the error
//     }
// }

// run();

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
