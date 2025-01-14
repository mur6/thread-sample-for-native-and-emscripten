console.log("save_as_png/main.js loaded");

import loadWASM from '/save_as_png/dist/save_as_png.js';

const run = async () => {
    // Load the wasm module and get the exports
    const wasmModule = await loadWASM();
    const Module = wasmModule;
    // Module.ccall('main', null, [], []);
    Module.ccall('captureImageFromCamera', null, [], []);
}

run();
