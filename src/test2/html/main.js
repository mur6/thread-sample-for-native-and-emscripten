import loadWASM from './async_test.js';

function pad(num, size) {
    // number to string
    let s = num.toString();
    while (s.length < size) {
        s = "0" + s;
    }
    return s;
}

async function load_test_images(Module) {
    const image_ids = [1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 13, 15];
    console.log('Image IDs:', image_ids);
    const filePaths = image_ids.map(
        (image_id) => `/Duncan_1_Foot_Left/zozomat_image_${pad(image_id, 4)}.in.jpg`
    );
    console.log('File paths:', filePaths);
    const buffers = [];
    for (const filePath of filePaths) {
        try {
            const uint8Array = await decodeJpegToUint8Array(Module, filePath);
            console.log('Decoded Uint8Array:', uint8Array);
            buffers.push(uint8Array);
        } catch (error) {
            console.error('Error decoding JPEG:', error);
        }
    }
    console.log('Buffers:', buffers);
    return [buffers, filePaths];
}


async function decodeJpegToUint8Array(Module, filePath) {
    // Emscriptenのファイルシステムからファイルを読み取る
    const fileData = Module.FS.readFile(filePath);

    // JPEG画像をデコードするために、ブラウザのImageオブジェクトを使用する
    const blob = new Blob([fileData], { type: 'image/jpeg' });
    const image = await createImageBitmap(blob);

    // div#container以下にcanvasを作成する.
    const container = document.getElementById('container');
    const canvas = document.createElement('canvas');
    container.appendChild(canvas);
    //Canvasを使用して画像を描画し、Uint8Arrayを取得する
    // const canvas = document.createElement('canvas');
    canvas.width = image.width;
    canvas.height = image.height;
    console.log('Canvas:', canvas);
    console.log(`Image: width: ${image.width}, height: ${image.height}`);
    const ctx = canvas.getContext('2d');
    ctx.drawImage(image, 0, 0);

    // ImageDataを取得し、Uint8Arrayに変換する
    const imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
    const rgbaData = imageData.data;

    // RGBAデータをBGRに変換
    const bgrData = new Uint8Array(rgbaData.length / 4 * 3);
    for (let i = 0; i < rgbaData.length; i += 4) {
        bgrData[i / 4 * 3] = rgbaData[i + 2];     // B
        bgrData[i / 4 * 3 + 1] = rgbaData[i + 1]; // G
        bgrData[i / 4 * 3 + 2] = rgbaData[i];     // R
    }

    const uint8Array = bgrData; // new Uint8Array(imageData.data.buffer);

    return uint8Array;
}


const run = async () => {
    const Module = await loadWASM();
    try {
        const result = await Module.asyncCalculation(42);
        console.log('result:', result);
        // ----------------- Init -----------------
        // document.getElementById('output').innerHTML += `<br>init start.`;
        // init(init_params, Module);
        // document.getElementById('output').innerHTML += `<br>init end.`;

    } catch (error) {
        console.log(error.stack);
        console.log('Failed to run the module:', error);
    }
    console.log('Module is ready:', Module);
};


run();
