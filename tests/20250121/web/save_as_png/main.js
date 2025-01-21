console.log("save_as_png/ fs of emscripten test");

import loadWASM from '/save_as_png/dist/my_em.js';
import { showFileList } from '/save_as_png/em_fs.js';


async function initVideo(Module) {
    const video = document.getElementById('videoElement');
    const canvas = document.getElementById('videoCanvas');
    const outputCanvas = document.getElementById('outputCanvas');
    outputCanvas.width = 720;
    outputCanvas.height = 1280;
    const captureButton = document.getElementById('captureButton');
    const processor = new Module.ImageProcessor();

    navigator.mediaDevices.getUserMedia({
        video: {facingMode: 'environment'},
        audio:false,
    }).then(stream => {
        video.srcObject = stream;
    });

    captureButton.addEventListener('click', () => {
        // Draw video frame to canvas
        canvas.width = video.videoWidth;
        canvas.height = video.videoHeight;
        const context = canvas.getContext('2d');
        context.drawImage(video, 0, 0, canvas.width, canvas.height);

        // Get image data from canvas
        const imageData = context.getImageData(0, 0, canvas.width, canvas.height);
        // C++で画像を処理
        const processedData = processImage(
            imageData.data,
            canvas.width,
            canvas.height
        );

        Module.SaveAsPngFromUint8Array("test.png", processedData);
        const processedImageData = new ImageData(
            new Uint8ClampedArray(processedData),
            720,
            1280
        );
        const ctx = outputCanvas.getContext('2d');
        ctx.putImageData(processedImageData, 0, 0);
    });
}

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
