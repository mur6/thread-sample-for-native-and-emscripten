// main.cpp
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <fstream>
#include <chrono>
#include <format>
#include <vector>
#include <cstdint>
#include <string>
#include <algorithm>
#include "lodepng.h" // Include LodePNG for PNG encoding

// ヒストグラムを計算するクラス
class ImageHistogram
{
private:
    std::vector<int> histogram;
    int width;
    int height;

public:
    ImageHistogram() : histogram(256, 0), width(0), height(0) {}

    void processImage(const std::vector<uint8_t> &imageData, int w, int h)
    {
        width = w;
        height = h;

        // ヒストグラムをリセット
        std::fill(histogram.begin(), histogram.end(), 0);

        // RGBAデータから輝度値を計算してヒストグラムを作成
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                int idx = (i * width + j) * 4;
                // グレースケール変換 (NTSC係数使用)
                uint8_t gray = static_cast<uint8_t>(
                    0.299 * imageData[idx] +     // R
                    0.587 * imageData[idx + 1] + // G
                    0.114 * imageData[idx + 2]   // B
                );
                histogram[gray]++;
            }
        }
    }

    std::vector<unsigned char> makePng()
    {
        std::vector<unsigned char> png;
        unsigned error = lodepng::encode(png, reinterpret_cast<const unsigned char *>(histogram.data()), width, height);
        if (error)
        {
            EM_ASM({ console.error('Error encoding PNG:', UTF8ToString($0)); }, lodepng_error_text(error));
        }
        return png;
    }

    std::vector<int> getHistogram() const
    {
        return histogram;
    }
};

// C++のインスタンスを保持
ImageHistogram *g_histogram = nullptr;

// JavaScriptから呼び出される関数
extern "C"
{
    EMSCRIPTEN_KEEPALIVE void initHistogram()
    {
        if (g_histogram)
            delete g_histogram;
        g_histogram = new ImageHistogram();
    }

    EMSCRIPTEN_KEEPALIVE void processImageData(uint8_t *data, int width, int height)
    {
        if (!g_histogram)
            return;
        std::vector<uint8_t> imageData(data, data + (width * height * 4));
        g_histogram->processImage(imageData, width, height);
    }

    EMSCRIPTEN_KEEPALIVE const int *getHistogramData()
    {
        if (!g_histogram)
            return nullptr;
        auto png = g_histogram->makePng();
        auto now = std::chrono::system_clock::now();
        std::string filename = std::format("hist_{:%Y%m%d_%H%M%S}.png", now);
        std::ofstream file(filename, std::ios::binary);
        file.write(reinterpret_cast<const char *>(png.data()), png.size());
        file.close();
        EM_ASM({ console.log('Error encoding PNG:', UTF8ToString($0)); }, filename.c_str());
        return g_histogram->getHistogram().data();
    }
}
