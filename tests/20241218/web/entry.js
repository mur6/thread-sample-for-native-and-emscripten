import loadWASM from '/dist/em_thread_and_event_loop.js';
console.log("entry.js loaded");

const calcAsync = async (Module) => {
    Module.startCalculation();

    return new Promise((resolve, reject) => {
        const address = Module.get_calculation_complete_address(); // アドレスを取得
        const HEAP8 = Module.HEAP8;
        const interval = setInterval(() => {
            const value = Atomics.load(HEAP8, address);
            const msg = `calculation_complete Status: ${value ? "Done" : "In Progress"}`;
            console.log(msg);
            if (value) {
                const result = Module.getCalculationResult();
                console.log('Calculation result:', result);
                clearInterval(interval);
                resolve(result);
            }
        }, 1000);
    });
}



const run = async () => {
    try{
        const Module = await loadWASM();
        console.log("Module.startCalculation(): start");
        const result = await calcAsync(Module);
        console.log(`Module.startCalculation(): end, result=${result}`);

        // const interval = setInterval(() => {
        //     console.log('interval test.');
        // }, 1000);
        // const address = Module.get_calculation_complete_address(); // アドレスを取得
        // const HEAP8 = new Int8Array(Module.HEAP8.buffer);
        // const interval2 = setInterval(() => {
        //     const value = Atomics.load(HEAP8, address);
        //     const msg = `calculation_complete Status: ${value ? "Done" : "In Progress"}`;
        //     console.log(msg);
        //     if (value) {
        //         console.log('Calculation result:', Module.getCalculationResult());
        //         clearInterval(interval2);
        //     }
        // }, 1000);


    } catch (error) {
        console.error("Error loading or running WASM:", error); // Catch and log the error
    }
}

run();
