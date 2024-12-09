import loadWASM from './async_test.js';

const removeFiles = async (FS) => {
    console.log('FS:', FS);
    FS.unlink("/outputs_wasm_2/my_scan_0000/test.txt");
    var names = FS.readdir("/outputs_wasm_2/my_scan_0000");
    console.log('/outputs_wasm_2/my_scan_0000: dirs:', names);
    FS.rmdir("/outputs_wasm_2/my_scan_0000");
    var names = FS.readdir("/outputs_wasm_2");
    console.log('/outputs_wasm_2: Names:', names);
}

function validate(init_params, buffer, Module) {
    const validate_info = new Module.ZsValidateInfo();
    // const buffer = new Uint8Array(100);
    const width = 720;
    const height = 1280;
    const state = Module.ZsStepByStepStates.CaptureTop;
    const result = Module.ValidateRgbImageFromUint8Array(
        init_params,
        validate_info,
        buffer,
        width,
        height,
        state)
    console.log('Validate Result:', result.success);
    return result;
}

function init (init_params, Module) {
    const r = Module.custom_zs_init(init_params);
    console.log('Init result:', r);
}




function pad(num, size) {
    // number to string
    let s = num.toString();
    while (s.length < size) {
        s = "0" + s;
    }
    return s;
}

async function load_test_images(Module) {
    // image files:
    // build_6/_deps/zozomattestdata-src/Reconstruct/Duncan_1_Foot_Left/zozomat_image_0001.in.jpg
    // build_6/_deps/zozomattestdata-src/Reconstruct/Duncan_1_Foot_Left/zozomat_image_0002.in.jpg
    // build_6/_deps/zozomattestdata-src/Reconstruct/Duncan_1_Foot_Left/zozomat_image_0003.in.jpg
    // build_6/_deps/zozomattestdata-src/Reconstruct/Duncan_1_Foot_Left/zozomat_image_0004.in.jpg
    // build_6/_deps/zozomattestdata-src/Reconstruct/Duncan_1_Foot_Left/zozomat_image_0006.in.jpg
    // build_6/_deps/zozomattestdata-src/Reconstruct/Duncan_1_Foot_Left/zozomat_image_0007.in.jpg
    // build_6/_deps/zozomattestdata-src/Reconstruct/Duncan_1_Foot_Left/zozomat_image_0008.in.jpg
    // build_6/_deps/zozomattestdata-src/Reconstruct/Duncan_1_Foot_Left/zozomat_image_0009.in.jpg
    // build_6/_deps/zozomattestdata-src/Reconstruct/Duncan_1_Foot_Left/zozomat_image_0010.in.jpg
    // build_6/_deps/zozomattestdata-src/Reconstruct/Duncan_1_Foot_Left/zozomat_image_0011.in.jpg
    // build_6/_deps/zozomattestdata-src/Reconstruct/Duncan_1_Foot_Left/zozomat_image_0013.in.jpg
    // build_6/_deps/zozomattestdata-src/Reconstruct/Duncan_1_Foot_Left/zozomat_image_0015.in.jpg

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



const run = async () => {
    const Module = await loadWASM();
    const [buffers, filePaths] = await load_test_images(Module);
    try {
        // removeFiles(Module.FS);
        // Module.callMain([]);
        document.getElementById('output').innerHTML = 'Module is ready';
        const scan_id = "my_scan_0000";
        const resourcePath = "/data";
        const workingPath = "/outputs_wasm_2";
        const appendage = Module.zs_left_right.ZS_SCAN_LEFT_APPENDAGE;
        const appType = Module.zs_app_type.ZS_APP_TYPE_CLIENT;
        // append div#output to message text with the result
        document.getElementById('output').innerHTML += `<br>init params.`;
        document.getElementById('output').innerHTML += `<br>scan_id: ${scan_id}`;
        document.getElementById('output').innerHTML += `<br>resourcePath: ${resourcePath}`;
        document.getElementById('output').innerHTML += `<br>workingPath: ${workingPath}`;
        document.getElementById('output').innerHTML += `<br>appendage: ${appendage}`;
        document.getElementById('output').innerHTML += `<br>appType: ${appType}`;

        const init_params = new Module.InitParams(scan_id, resourcePath, workingPath, appendage, appType);

        // ----------------- Init -----------------
        document.getElementById('output').innerHTML += `<br>init start.`;
        init(init_params, Module);
        document.getElementById('output').innerHTML += `<br>init end.`;

        setTimeout(async () => {
            // ----------------- Validate -----------------
            for (let i = 0; i < buffers.length; i++) {
                const filePath = filePaths[i];
                document.getElementById('output').innerHTML += `<br>validate start for file: ${filePath}`;
                const buffer = buffers[i];
                const result = validate(init_params, buffer, Module);
                document.getElementById('output').innerHTML += `<br>validate end:  result.success=${result.success}`;
                // wait 3 seconds
                await new Promise((resolve) => setTimeout(resolve, 900));
            }
            // document.getElementById('output').innerHTML += `<br>validate start.`;
            // const filePath = "/Duncan_1_Foot_Left/zozomat_image_0002.in.jpg";
            // decodeJpegToUint8Array(Module, filePath).then((buffer) => {
            //     validate(init_params, buffer, Module);
            // });
            // document.getElementById('output').innerHTML += `<br>validate end.`;
            console.log('%%%%%%%: Validate end.');
            setTimeout(async () => {
                console.log('%%%%%%%: Foot Model start.');
                document.getElementById('output').innerHTML += `<br>Foot Model start.`;
                const footModel = Module.BuildAndReturnFootModel(scan_id, init_params);
                console.log('Foot Model:', footModel);
                // .property("header", &FootBin::header)
                // .property("scanData", &FootBin::scanData)
                // .property("mesh", &FootBin::mesh)
                // .property("measurements", &FootBin::measurements)
                // .property("footInfo", &FootBin::footInfo)
                const header = footModel.header;
                console.log('Header:', header);
                const scanData = footModel.scanData;
                console.log('Scan Data:', scanData);
                const mesh = footModel.mesh;
                console.log('Mesh:', mesh);
                const measurements = Module.convertFootBinForDebug(footModel);
                for (var i = 0; i < measurements.size(); i++) {
                    console.log("Measurement:", measurements.get(i));
                }
                console.log('Measurements:', measurements);
                const footInfo = footModel.footInfo;
                console.log('Foot Info:', footInfo);
                document.getElementById('output').innerHTML += `<br>Foot Model end.`;
            }, 3000);
        }, 3000);


    } catch (error) {
        console.log(error.stack);
    console.log('Failed to run the module:', error);
    }
  console.log('Module is ready:', Module);
};


// const stat_result = FS.stat('/data');
// console.log('Stat result:', stat_result);
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

async function image_load_test() {
    const Module = await loadWASM();
    try {
        const filePath = "/Duncan_1_Foot_Left/zozomat_image_0001.in.jpg";
        const uint8Array = await decodeJpegToUint8Array(Module, filePath);
        console.log('Decoded Uint8Array:', uint8Array);
    } catch (error) {
        console.error('Error decoding JPEG:', error);
    }
}


run();
// image_load_test();
