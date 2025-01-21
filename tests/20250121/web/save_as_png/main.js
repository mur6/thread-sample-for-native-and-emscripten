console.log("save_as_png/ fs of emscripten test");

import loadWASM from '/save_as_png/dist/my_em.js';
import { showFileList } from '/save_as_png/em_fs.js';

const run = async () => {
    // Load the wasm module and get the exports
    const wasmModule = await loadWASM();
    const Module = wasmModule;
    console.log("Module loaded:", Module);

    const currentPath = '/working/';
    document.getElementById('showFilesButton').addEventListener('click', () => {
        console.log("testFunc called");
        Module.ccall('makeOnePngFile', null, [], []);
        showFileList(Module.FS, currentPath);
    });
}

run();
