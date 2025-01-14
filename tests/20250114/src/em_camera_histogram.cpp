// main.cpp
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <vector>
#include <cstdint>

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
        return g_histogram->getHistogram().data();
    }
}
