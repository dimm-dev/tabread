#include <algorithm>
#include <execution>
#include <functional>

#include <opencv2/imgproc.hpp>

#include "image_filter.h"
#include "task_executor.h"

namespace tabread
{
namespace tasks
{

Sequental_task_executor::Sequental_task_executor(imgproc::OCR_rect_filter& ocr_filter,
    text::Text_filter& text_filter,
        std::shared_ptr<text::Recognize> ocr_engine) :
    _ocr_filter{ ocr_filter },
        _text_filter{ text_filter },
        _ocr_engine{ ocr_engine }
{
}

std::vector<std::vector<std::string>> Sequental_task_executor::execute(const cv::Mat& image, const imgproc::Table& table)
{
    std::vector<std::vector<std::string>> result;
    auto text_func = [ &result, &image, this ](const imgproc::Rects& row)
    {
        result.emplace_back(execute(image, row));
    };

    std::for_each(table.begin(), table.end(), text_func);

    return result;
}

std::vector<std::string> Sequental_task_executor::execute(const cv::Mat& image, const imgproc::Rects& row)
{
    std::vector<std::string> row_text;
    for (const auto& cell : row)
    {
        _ocr_filter.execute(image, cell, _cell_image);
        std::string cell_text { _ocr_engine->text(_cell_image) };
        _text_filter.execute(cell_text);
        row_text.emplace_back(std::move(cell_text));
    }
    return row_text;
}

Async_task_executor::Task_slice::Task_slice(int rn,
    std::vector<std::string>* text,
        const imgproc::Rects* r) :
    row_num{ rn },
        rects{ r },
        row_text{ text },
        failed{ false }
{
}

Async_task_executor::Async_task_executor(imgproc::OCR_rect_filter& ocr_filter,
    text::Text_filter& text_filter,
        actions::Resource_provider_base<text::Recognize>& engine_provider) :
    _ocr_filter(ocr_filter),
        _text_filter { text_filter },
        _engine_provider(engine_provider)
{
}

std::vector<std::vector<std::string>> Async_task_executor::execute(const cv::Mat& image, const imgproc::Table& table)
{
    std::vector<Task_slice> task_data(table.size());
    std::vector<std::vector<std::string>> text(table.size());
    auto gen_func = [ row = 0, this, &text, &table ]() mutable
    {
        Task_slice slice { row, &text[row], &table[row] };
        ++row;
        return slice;
    };
    std::generate(task_data.begin(), task_data.end(), gen_func);
    std::function<void(Task_slice&)> task_func = std::bind(&Async_task_executor::task, this, image, std::placeholders::_1);
    std::for_each(std::execution::par, task_data.begin(), task_data.end(), task_func);

    if (std::find_if(task_data.begin(), task_data.end(), [](const Task_slice& s) { return s.failed; }) != task_data.end())
        throw std::runtime_error("Async_task_executor: one or more of OCR tasks failed");

    return text;
}

void Async_task_executor::task(const cv::Mat& image, Task_slice& slice)
{
    try
    {
        Sequental_task_executor te { _ocr_filter, _text_filter, _engine_provider.create() };
        *slice.row_text = te.execute(image, *slice.rects);
    }
    catch (const std::exception& e)
    {
        slice.failed = true;
    }
}

}
}
