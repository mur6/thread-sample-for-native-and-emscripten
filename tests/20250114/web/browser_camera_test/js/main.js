console.log("browser_camera_test/js/main.js loaded");

import loadWASM from '/browser_camera_test/dist/em_camera_histogram.js';


async function init(wasmModule) {
    const video = document.getElementById('videoElement');
    const stream = await navigator.mediaDevices.getUserMedia({ video: true });
    video.srcObject = stream;

    // 隠れたcanvasを作成（画像キャプチャ用）
    const captureCanvas = document.createElement('canvas');
    captureCanvas.width = 640;
    captureCanvas.height = 480;
    const captureCtx = captureCanvas.getContext('2d');

    // ヒストグラム表示用のcanvas
    const histogramCanvas = document.getElementById('histogramCanvas');
    const histogramCtx = histogramCanvas.getContext('2d');

    document.getElementById('captureButton').onclick = function() {
        // ビデオフレームをキャプチャ
        captureCtx.drawImage(video, 0, 0, captureCanvas.width, captureCanvas.height);
        const imageData = captureCtx.getImageData(0, 0, captureCanvas.width, captureCanvas.height);

        // C++の処理を呼び出し
        const Module = wasmModule;
        Module.ccall('initHistogram', null, [], []);
        
        // TypedArrayをC++のメモリに転送
        const dataPtr = Module._malloc(imageData.data.length);
        Module.HEAPU8.set(imageData.data, dataPtr);
        
        Module.ccall('processImageData', null, 
            ['number', 'number', 'number'],
            [dataPtr, captureCanvas.width, captureCanvas.height]
        );

        // ヒストグラムデータを取得
        const histogramPtr = Module.ccall('getHistogramData', 'number', [], []);
        const histogramData = new Int32Array(Module.HEAP32.buffer, histogramPtr, 256);

        // メモリ解放
        Module._free(dataPtr);

        // ヒストグラムを描画
        drawHistogram(histogramData);
    };

    function drawHistogram(histogramData) {
        // 最大値を見つける
        const maxVal = Math.max(...histogramData);

        // キャンバスをクリア
        histogramCtx.fillStyle = 'white';
        histogramCtx.fillRect(0, 0, histogramCanvas.width, histogramCanvas.height);

        // ヒストグラムを描画
        histogramCtx.fillStyle = 'rgb(0, 0, 255)';
        const binWidth = histogramCanvas.width / 256;

        for (let i = 0; i < 256; i++) {
            const height = (histogramData[i] / maxVal) * histogramCanvas.height;
            histogramCtx.fillRect(
                i * binWidth,
                histogramCanvas.height - height,
                binWidth,
                height
            );
        }
    }
}

const run = async () => {
    // Load the wasm module and get the exports
    const wasmModule = await loadWASM();
    await init(wasmModule);
}

run();
