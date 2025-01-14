console.log('entry_filesystem.js loaded');

import loadWASM from '/dist/em_filesystem.js';

const run = async () => {
    try {
        const hook_func = (path, scriptDirectory) => {
            console.log(`hook_func: path=${path}, scriptDirectory=${scriptDirectory}`);
            if (path === 'em_filesystem.data') {
                scriptDirectory = '/dist/';
            }
            return scriptDirectory + path;
        }
        const Module = await loadWASM({ locateFile: hook_func });
        console.log(Module);
    } catch (error) {
        console.error("Error loading or running WASM:", error); // Catch and log the error
    }
};

run();

