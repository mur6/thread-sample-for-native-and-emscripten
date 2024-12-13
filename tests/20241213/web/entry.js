import loadWASM from '/dist/main.js';

const run = async () => {
    setTimeout(async () => {
        const Module = await loadWASM();
        console.log(Module);
    }, 3000);
};

run();
