#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>
#include <emscripten/html5.h>
#include <vector>
#include <chrono>
#include <string>
#include <format>
#include <fstream>
#include <algorithm>
#include <cstdint>
#include <iostream>
#include "lodepng.h" // Include LodePNG for PNG encoding



// 画像データを処理するメインメソッド
emscripten::val ImageProcessor::processImage(const emscripten::val &inputData, int inputWidth, int inputHeight)
{
    // 入力データをC++のvectorに変換
    std::vector<unsigned char> imageData = convertJSArrayToVector(inputData);
    // vector の 要素0 の値をプリント
    std::cout << "imageData[0]: " << imageData[0] << std::endl;

    // 目標のアスペクト比を計算
    const double targetAspectRatio = 720.0 / 1280.0;
    //const double inputAspectRatio = static_cast<double>(inputWidth) / inputHeight;

    // 切り出すサイズを計算
    int cropWidth, cropHeight;
    calculateCropDimensions(inputWidth, inputHeight, targetAspectRatio,
                            cropWidth, cropHeight);

    // 切り出し開始位置を計算（中央に配置）
    int startX = (inputWidth - cropWidth) / 2;
    int startY = (inputHeight - cropHeight) / 2;

    // 画像を切り出してリサイズ
    std::vector<unsigned char> croppedData = cropAndResize(
        imageData, inputWidth, inputHeight,
        startX, startY, cropWidth, cropHeight,
        720, 1280);
    std::cout << "croppedData[0]: " << croppedData[0] << std::endl;
    // 結果をJavaScript側に返す
    return convertVectorToJSArray(croppedData);
}


// JavaScriptの配列をC++のvectorに変換
std::vector<unsigned char> ImageProcessor::convertJSArrayToVector(const emscripten::val &jsArray)
{
    const auto length = jsArray["length"].as<unsigned>();
    std::vector<unsigned char> result(length);

    emscripten::val memory = emscripten::val::module_property("HEAPU8");
    emscripten::val memoryView = jsArray["constructor"].new_(memory["buffer"]);

    for (unsigned i = 0; i < length; ++i)
    {
        result[i] = jsArray[i].as<unsigned char>();
    }

    return result;
}

// C++のvectorをJavaScript配列に変換
emscripten::val ImageProcessor::convertVectorToJSArray(const std::vector<unsigned char> &vec)
{
    emscripten::val Uint8Array = emscripten::val::global("Uint8Array");
    emscripten::val result = Uint8Array.new_(vec.size());

    for (size_t i = 0; i < vec.size(); ++i)
    {
        result.set(i, vec[i]);
    }

    return result;
}

// 切り出しサイズを計算
void ImageProcessor::calculateCropDimensions(int inputWidth, int inputHeight,
                                double targetAspectRatio,
                                int &cropWidth, int &cropHeight)
{
    if (static_cast<double>(inputWidth) / inputHeight > targetAspectRatio)
    {
        // 入力画像が横長の場合
        cropHeight = inputHeight;
        cropWidth = static_cast<int>(inputHeight * targetAspectRatio);
    }
    else
    {
        // 入力画像が縦長の場合
        cropWidth = inputWidth;
        cropHeight = static_cast<int>(inputWidth / targetAspectRatio);
    }
}

// 画像の切り出しとリサイズを行う
std::vector<unsigned char> ImageProcessor::cropAndResize(
    const std::vector<unsigned char> &input,
    int inputWidth, int inputHeight,
    int startX, int startY,
    int cropWidth, int cropHeight,
    int targetWidth, int targetHeight)
{

    std::vector<unsigned char> result(targetWidth * targetHeight * 4);

    for (int y = 0; y < targetHeight; ++y)
    {
        for (int x = 0; x < targetWidth; ++x)
        {
            // ソース画像での位置を計算
            float srcX = startX + (x * static_cast<float>(cropWidth) / targetWidth);
            float srcY = startY + (y * static_cast<float>(cropHeight) / targetHeight);

            // バイリニア補間で色を計算
            int x1 = static_cast<int>(srcX);
            int y1 = static_cast<int>(srcY);
            int x2 = std::min(x1 + 1, inputWidth - 1);
            int y2 = std::min(y1 + 1, inputHeight - 1);

            float xWeight = srcX - x1;
            float yWeight = srcY - y1;

            for (int c = 0; c < 4; ++c)
            {
                float val =
                    (1 - xWeight) * (1 - yWeight) * input[(y1 * inputWidth + x1) * 4 + c] +
                    xWeight * (1 - yWeight) * input[(y1 * inputWidth + x2) * 4 + c] +
                    (1 - xWeight) * yWeight * input[(y2 * inputWidth + x1) * 4 + c] +
                    xWeight * yWeight * input[(y2 * inputWidth + x2) * 4 + c];

                result[(y * targetWidth + x) * 4 + c] = static_cast<unsigned char>(val);
            }
        }
    }

    return result;
}

// extern "C"
// {
//     // Function to capture the image from the camera
//     EMSCRIPTEN_KEEPALIVE
//     void captureImageFromCamera()
//     {
//         EM_ASM({
//             const video = document.createElement('video');
//             const canvas = document.createElement('canvas');
//             const context = canvas.getContext('2d');

//             navigator.mediaDevices.getUserMedia({video : true}).then(stream = > {
//                 video.srcObject = stream;
//                 video.play();

//                 video.addEventListener('loadeddata', () = > {
//                     canvas.width = video.videoWidth;
//                     canvas.height = video.videoHeight;
//                     context.drawImage(video, 0, 0);
//                     video.pause();
//                     video.srcObject.getTracks().forEach(track => track.stop());
//                     const imageData = context.getImageData(0, 0, canvas.width, canvas.height);
//                     const buffer = new Uint8Array(imageData.data.buffer);
//                     Module.ccall('processImage', null, ['array', 'number', 'number'], [buffer, canvas.width, canvas.height]); });
//             });
//         });
//     }

//     EMSCRIPTEN_KEEPALIVE void makeOnePngFile()
//     {
//         int width = 256;
//         int height = 256;
//         uint8_t *buffer = new uint8_t[width * height * 4];
//         for (int i = 0; i < width * height * 4; i += 4)
//         {
//             buffer[i] = 0;       // R
//             buffer[i + 1] = 0;   // G
//             buffer[i + 2] = 255; // B
//             buffer[i + 3] = 255; // A
//         }
//         std::vector<unsigned char> png;
//         unsigned error = lodepng::encode(png, buffer, width, height);
//         if (error)
//         {
//             EM_ASM({ console.error('Error encoding PNG:', UTF8ToString($0)); }, lodepng_error_text(error));
//             return;
//         }
//         std::ofstream file("/working/blue_image.png", std::ios::binary);
//         file.write(reinterpret_cast<const char *>(png.data()), png.size());
//         file.close();
//         EM_ASM({ console.log('File saved to /working/blue_image.png'); });
//     }

//     EMSCRIPTEN_KEEPALIVE void makeOneTextFile()
//     {
//         auto now = std::chrono::system_clock::now();
//         auto now_sec = std::chrono::time_point_cast<std::chrono::seconds>(now);
//         std::string filename = std::format("hist_{:%Y%m%d_%EX}.txt", now_sec);
//         // make one text file in the virtual filesystem
//         std::ofstream file("/working/" + filename);
//         file << "Hello, World!";
//         file.close();
//         EM_ASM({ console.log('File saved to /working/$0', UTF8ToString($0)); }, filename.c_str());
//     }
// }

// EMSCRIPTEN_KEEPALIVE
// int main()
// {
//     // Mount the Emscripten filesystem
//     EM_ASM({
//         FS.mkdir('/working');
//         FS.mount(MEMFS, {}, '/working');
//         FS.syncfs(true, function(err) { console.log('Filesystem synced'); });
//     });

//     return 0;
// }

void SaveAsPngFromUint8Array(const emscripten::val& uint8Array, int width, int height)
{
    if (!uint8Array.instanceof(emscripten::val::global("Uint8Array"))) {
        throw std::runtime_error("Input must be Uint8Array");
    }

    unsigned int length = uint8Array["length"].as<unsigned int>();
    std::vector<uint8_t> buffer;
    buffer.reserve(length);

    // Convert Uint8Array to vector<uint8_t> using typed array
    emscripten::val typedArray = uint8Array["constructor"].new_(
        emscripten::val::global("ArrayBuffer").new_(length)
    );
    typedArray.call<void>("set", uint8Array);
    // Copy the data
    for (unsigned int i = 0; i < length; ++i) {
        buffer[i] = typedArray[i].as<uint8_t>();
    }

    // Encode the image as PNG using LodePNG
    std::vector<unsigned char> png;
    unsigned error = lodepng::encode(png, buffer, width, height);
    if (error)
    {
        std::cerr << "Error encoding PNG: " << lodepng_error_text(error) << std::endl;
        return;
    }

    // Save the PNG image to the Emscripten virtual file system
    // replace "/" in filename with "_"
    std::string safeFilename = "test.png";
    std::replace(safeFilename.begin(), safeFilename.end(), '/', '_');
    std::ofstream file("/outputs_wasm_2/" + safeFilename, std::ios::binary);
    file.write(reinterpret_cast<const char *>(png.data()), png.size());
    file.close();
}

EMSCRIPTEN_BINDINGS(my_module)
{
    function("SaveAsPngFromUint8Array", &SaveAsPngFromUint8Array);
}
