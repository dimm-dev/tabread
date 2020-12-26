#pragma once

#include <string>
#include <vector>

#include "contour_filter.h"
#include "ocr_filter.h"
#include "text_filter.h"

#include "resource_provider.h"

#include "recognize.h"

namespace tabread
{
namespace tasks
{

class Task_executor
{
    public:
        virtual std::vector<std::vector<std::string>> execute(const cv::Mat& image, const imgproc::Table& table) = 0;
};

class Sequental_task_executor : public Task_executor
{
    public:
        Sequental_task_executor(imgproc::OCR_rect_filter& ocr_filter, text::Text_filter& text_filter, std::shared_ptr<text::Recognize> ocr_engine);

        std::vector<std::vector<std::string>> execute(const cv::Mat& image, const imgproc::Table& table) override;
        std::vector<std::string> execute(const cv::Mat& image, const imgproc::Rects& row);

    private:
        cv::Mat _image_buf;
        cv::Mat _cell_image;
        imgproc::OCR_rect_filter& _ocr_filter;
        text::Text_filter& _text_filter;
        std::shared_ptr<text::Recognize> _ocr_engine;
};

class Async_task_executor : public Task_executor
{
    public:
        Async_task_executor(imgproc::OCR_rect_filter& ocr_filter, text::Text_filter& text_filter, actions::Resource_provider_base<text::Recognize>& engine_provider);
        std::vector<std::vector<std::string>> execute(const cv::Mat& image, const imgproc::Table& table) override;

    private:
        struct Task_slice
        {
            int row_num;
            const imgproc::Rects* rects;
            std::vector<std::string>* row_text;

            bool failed;

            Task_slice(int rn = 0, std::vector<std::string>* text = nullptr, const imgproc::Rects* r = nullptr);
        };

        imgproc::OCR_rect_filter& _ocr_filter;
        text::Text_filter& _text_filter;
        actions::Resource_provider_base<text::Recognize>& _engine_provider;

        void task(const cv::Mat& image, Task_slice& slice);
};

}
}
