console.log("save_as_png/ fs of emscripten test");

import loadWASM from '/save_as_png/dist/my_em.js';
import { showFileList } from '/save_as_png/em_fs.js';


function calculateCropDimensions(
    inputWidth,
    inputHeight,
    targetAspectRatio
) {

    const inputAspectRatio = inputWidth / inputHeight;

    let cropWidth, cropHeight;
    if (inputAspectRatio > targetAspectRatio) {
        // 入力画像が目標のアスペクト比よりも横長の場合
        cropHeight = inputHeight;
        cropWidth = parseInt(inputHeight * targetAspectRatio, 10);
    } else {
        // 入力画像が目標のアスペクト比よりも縦長の場合
        cropWidth = inputWidth;
        cropHeight = parseInt(inputWidth / targetAspectRatio, 10);
    }

    return [cropWidth, cropHeight];
}

// std::vector<unsigned char> cropAndResize(const std::vector<unsigned char>& input, int inputWidth, int inputHeight, int startX, int startY, int cropWidth, int cropHeight, int targetWidth, int targetHeight);
function cropAndResize(input, inputWidth, inputHeight, startX, startY, cropWidth, cropHeight, targetWidth, targetHeight) {
    const croppedData = [];
    for (let y = 0; y < targetHeight; y++) {
        for (let x = 0; x < targetWidth; x++) {
            const inputX = startX + parseInt(x * cropWidth / targetWidth, 10);
            const inputY = startY + parseInt(y * cropHeight / targetHeight, 10);
            const inputIndex = (inputY * inputWidth + inputX) * 4;
            croppedData.push(input[inputIndex]);
            croppedData.push(input[inputIndex + 1]);
            croppedData.push(input[inputIndex + 2]);
            croppedData.push(input[inputIndex + 3]);
        }
    }
    return croppedData;
}

const target_width = 720;
const target_height = 1280;
// 目標のアスペクト比
const targetAspectRatio = target_width / target_height;

function cropCenterOfImage(inputData, inputWidth, inputHeight) {
    // 入力データの長さ
    console.log('inputData.length=', inputData.length);
    // 入力データの１つ目の要素をdebug出力
    console.log('inputData[0]=', inputData[0]);
    // 切り出すサイズを計算
    const [cropWidth, cropHeight] = calculateCropDimensions(inputWidth, inputHeight, targetAspectRatio);
    // 切り出し開始位置を計算（中央に配置）
    const startX = (inputWidth - cropWidth) / 2;
    const startY = (inputHeight - cropHeight) / 2;

    // 画像を切り出してリサイズ
    const croppedData = cropAndResize(
        inputData, inputWidth, inputHeight,
        startX, startY, cropWidth, cropHeight,
        target_width, target_height);
    const resultData = Uint8Array.from(croppedData);
    // 処理結果の長さをdebug出力
    console.log('resultData.length=', resultData.length);
    // 処理結果の１つ目の要素をdebug出力
    console.log('resultData[0]=', resultData[0]);
    return resultData;
}

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
        // jsで画像の中央部を切り出す処理
        const processedData = cropCenterOfImage(
            imageData.data,
            target_width, target_height
        );

        // Module.SaveAsPngFromUint8Array(processedData, target_width, target_height);
        const processedImageData = new ImageData(
            new Uint8ClampedArray(processedData),
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
    initVideo(Module);

    const currentPath = '/working/';
    document.getElementById('showFilesButton').addEventListener('click', () => {
        console.log("showFilesButton clicked");
        showFileList(Module.FS, currentPath);
    });
}

run();
