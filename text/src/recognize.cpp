#ifndef WIN32
#  include <stdlib.h>
#endif

#include <filesystem>

#include <tesseract/baseapi.h>

#include "recognize.h"

namespace
{

void release_tesseract(void* arg)
{
    auto api = reinterpret_cast<tesseract::TessBaseAPI*>(arg);
    api->End();
    delete api;
}

}

namespace tabread
{
namespace text
{

void Recognize::initialize(const std::string& bin_path)
{
    std::filesystem::path path{bin_path};
#ifdef WIN32
    _putenv_s("TESSDATA_PREFIX", path.append("tessdata").string().c_str());
#else
    setenv("TESSDATA_PREFIX", path.append("tessdata").string().c_str(), 1);
#endif
}

Recognize::Recognize() : _ocr_engine{ new tesseract::TessBaseAPI(), release_tesseract }
{
    if (_ocr_engine->Init(NULL, "rus+eng+equ"))
        throw std::runtime_error("Unable to initialize tesseract API");
    _ocr_engine->SetVariable("psm", "13");
}

std::string Recognize::text(const cv::Mat& image)
{
    _ocr_engine->SetImage(image.data, image.cols, image.rows, image.channels(), (int)image.step);
    _ocr_engine->Recognize(0);
	std::unique_ptr<char> outText(_ocr_engine->GetUTF8Text());
	std::string text{ outText.get() };
    return text;
}

Recognize::~Recognize() noexcept
{
}

}
}
