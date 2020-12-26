#pragma once

#include <opencv2/imgproc.hpp>

// TODO: remove dependency to OpenCV

namespace tesseract
{
class TessBaseAPI;
}

namespace tabread
{
namespace text
{

class Recognize
{
    public:
        static void initialize(const std::string& bin_path);
        Recognize();
        ~Recognize() noexcept;
        std::string text(const cv::Mat& image);

    private:
        std::unique_ptr<tesseract::TessBaseAPI, std::function<void(void*)>> _ocr_engine;
};

}
}
