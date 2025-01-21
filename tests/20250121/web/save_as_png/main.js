console.log("save_as_png/ fs of emscripten test");

import loadWASM from '/save_as_png/dist/my_em.js';
import { showFileList } from '/save_as_png/em_fs.js';

const target_width = 720;
const target_height = 1280;

async function initVideo(Module) {
    const video = document.getElementById('videoElement');
    const canvas = document.getElementById('videoCanvas');
    const outputCanvas = document.getElementById('outputCanvas');
    outputCanvas.width = target_width;
    outputCanvas.height = target_height;
    const captureButton = document.getElementById('captureButton');

    navigator.mediaDevices.getUserMedia({
        video: { facingMode: 'environment' },
        audio: false,
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
        // emscripten(c++)で画像の中央部を切り出す処理
        // const processedData = cropCenterOfImage(
        //     imageData.data,
        //     target_width, target_height
        // );
        console.log("imageData:", imageData);
        console.log("imageData.length:", imageData.length);
        const inputData = imageData.data;
        // 画像を切り出してリサイズ
        // emscripten::val cropAndResizeImage(
        //     const emscripten::val &inputData,
        //     int inputWidth, int inputHeight,
        //     int outputWidth, int outputHeight);

        const croppedData = Module.cropAndResizeImage(
            inputData, canvas.width, canvas.height,
            target_width, target_height);
        console.log("croppedData:", croppedData);
        console.log("croppedData.length:", croppedData.length);
        Module.SaveAsPngFromUint8Array(croppedData, target_width, target_height);
        const processedImageData = new ImageData(
            new Uint8ClampedArray(croppedData),
            target_width, target_height);
        const ctx = outputCanvas.getContext('2d');
        ctx.putImageData(processedImageData, 0, 0);
    });
}

const run = async () => {
    // Load the wasm module and get the exports
    const wasmModule = await loadWASM();
    const Module = wasmModule;
    console.log("Module loaded:", Module);

    const workingDirPath = '/working/';
    FS.mkdir(workingDirPath);
    FS.mount(MEMFS, {}, workingDirPath);
    FS.syncfs(true, function(err) { console.log('Filesystem synced'); });

    initVideo(Module, cropedDataHook);

    document.getElementById('showFilesButton').addEventListener('click', () => {
        console.log("showFilesButton clicked");
        showFileList(Module.FS, workingDirPath);
    });
}

run();
