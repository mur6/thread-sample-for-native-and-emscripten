#include <emscripten.h>
#include <emscripten/html5.h>
#include <vector>
#include <chrono>
#include <string>
#include <format>
#include <fstream>
#include <algorithm>
#include <cstdint>
#include "lodepng.h" // Include LodePNG for PNG encoding

// Function to process the image and enhance red color
void enhanceRed(uint8_t *buffer, int width, int height)
{
    int numPixels = width * height;
    for (int i = 0; i < numPixels; ++i)
    {
        int offset = i * 4;
        uint8_t blue = buffer[offset];
        uint8_t green = buffer[offset + 1];
        uint8_t red = buffer[offset + 2];

        // Simple enhancement: amplify the red channel
        buffer[offset + 2] = std::min(255, red + 50);
    }
}

extern "C"
{
    // Function to capture the image from the camera
    EMSCRIPTEN_KEEPALIVE
    void captureImageFromCamera()
    {
        EM_ASM({
            const video = document.createElement('video');
            const canvas = document.createElement('canvas');
            const context = canvas.getContext('2d');

            navigator.mediaDevices.getUserMedia({video : true}).then(stream => {
                video.srcObject = stream;
                video.play();

                video.addEventListener('loadeddata', () => {
                    canvas.width = video.videoWidth;
                    canvas.height = video.videoHeight;
                    context.drawImage(video, 0, 0);
                    video.pause();
                    video.srcObject.getTracks().forEach(track => track.stop());
                    
                    const imageData = context.getImageData(0, 0, canvas.width, canvas.height);
                    const buffer = new Uint8Array(imageData.data.buffer);
                    
                    Module.ccall('processImage', null, ['array', 'number', 'number'], [buffer, canvas.width, canvas.height]); });
            });
        });
    }

    // Function to process the image in C++
    EMSCRIPTEN_KEEPALIVE
    void processImage(uint8_t *buffer, int width, int height)
    {
        // Enhance red color
        enhanceRed(buffer, width, height);

        // Encode the image as PNG using LodePNG
        std::vector<unsigned char> png;
        unsigned error = lodepng::encode(png, buffer, width, height);
        if (error)
        {
            EM_ASM({ console.error('Error encoding PNG:', UTF8ToString($0)); }, lodepng_error_text(error));
            return;
        }

        // Save the PNG image to the Emscripten virtual file system
        std::ofstream file("/red_enhanced_image.png", std::ios::binary);
        file.write(reinterpret_cast<const char *>(png.data()), png.size());
        file.close();

        EM_ASM({
            console.log('Image saved to /red_enhanced_image.png');
        });
    }

    EMSCRIPTEN_KEEPALIVE void makeOnePngFile()
    {
        int width = 256;
        int height = 256;
        uint8_t *buffer = new uint8_t[width * height * 4];
        for (int i = 0; i < width * height * 4; i += 4)
        {
            buffer[i] = 0;     // R
            buffer[i + 1] = 0; // G
            buffer[i + 2] = 255; // B
            buffer[i + 3] = 255; // A
        }
        std::vector<unsigned char> png;
        unsigned error = lodepng::encode(png, buffer, width, height);
        if (error)
        {
            EM_ASM({ console.error('Error encoding PNG:', UTF8ToString($0)); }, lodepng_error_text(error));
            return;
        }
        std::ofstream file("/working/blue_image.png", std::ios::binary);
        file.write(reinterpret_cast<const char *>(png.data()), png.size());
        file.close();
        EM_ASM({ console.log('File saved to /working/blue_image.png'); });
    }

    EMSCRIPTEN_KEEPALIVE void makeOneTextFile()
    {
        auto now = std::chrono::system_clock::now();
        auto now_sec = std::chrono::time_point_cast<std::chrono::seconds>(now);
        std::string filename = std::format("hist_{:%Y%m%d_%EX}.txt", now_sec);
        // make one text file in the virtual filesystem
        std::ofstream file("/working/" + filename);
        file << "Hello, World!";
        file.close();
        EM_ASM({ console.log('File saved to /working/$0', UTF8ToString($0)); }, filename.c_str());
    }
}

EMSCRIPTEN_KEEPALIVE
int main()
{
    // Mount the Emscripten filesystem
    EM_ASM({
        FS.mkdir('/working');
        FS.mount(MEMFS, {}, '/working');
        FS.syncfs(true, function(err) { console.log('Filesystem synced'); });
    });

    return 0;
}
