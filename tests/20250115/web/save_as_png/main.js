console.log("save_as_png/ fs of emscripten test");

import loadWASM from '/save_as_png/dist/my_em.js';
import { showFileList, downloadFile } from '/save_as_png/em_fs.js';

const run = async () => {
    // Load the wasm module and get the exports
    const wasmModule = await loadWASM();
    const Module = wasmModule;
    console.log("Module loaded:", Module);
    // Module.ccall('main', null, [], []);
    const currentPath = '/working/';
    const FS = Module.FS;
    const files = FS.readdir(currentPath);

    const fileListDiv = document.getElementById('fileList');
    fileListDiv.innerHTML = ''; // Clear previous content

    // Sort files: directories first, then regular files
    const sortedFiles = files.sort((a, b) => {
        if (a === '.' || a === '..') return -1;
        if (b === '.' || b === '..') return 1;

        const statA = FS.stat(currentPath + a);
        const statB = FS.stat(currentPath + b);
        const isDirectoryA = FS.isDir(statA.mode);
        const isDirectoryB = FS.isDir(statB.mode);

        if (isDirectoryA && !isDirectoryB) return -1;
        if (!isDirectoryA && isDirectoryB) return 1;
        return a.localeCompare(b);
    });

    for (const file of sortedFiles) {
        if (file === '.' || file === '..') continue;

        const fullPath = currentPath + file;
        const stat = FS.stat(fullPath);
        const isDirectory = FS.isDir(stat.mode);

        const fileDiv = document.createElement('div');
        fileDiv.className = `file-item ${isDirectory ? 'directory' : 'file'}`;
        fileDiv.textContent = file + (isDirectory ? '/' : '');

        if (isDirectory) {
            fileDiv.onclick = () => showFileList(fullPath + '/');
        } else {
            fileDiv.onclick = () => downloadFile(fullPath);
        }

        fileListDiv.appendChild(fileDiv);
    }

}

run();
