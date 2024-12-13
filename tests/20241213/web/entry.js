import loadWASM from '/dist/main.js';

const run = async () => {
    const Module = await loadWASM();
    console.log(Module);
};

run();
