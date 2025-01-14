console.log("save_as_png/ fs of emscripten test");

import loadWASM from '/save_as_png/dist/my_em.js';

const run = async () => {
    // Load the wasm module and get the exports
    const wasmModule = await loadWASM();
    const Module = wasmModule;
    console.log("Module loaded:", Module);
    Module.ccall('main', null, [], []);
    // Module.ccall('captureImageFromCamera', null, [], []);
}

run();
