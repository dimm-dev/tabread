#pragma once

#include <fstream>
#include <functional>
#include <list>
#include <memory>

#include "resource_provider.h"

#include "contour_filter.h"
#include "image_filter.h"
#include "ocr_filter.h"

#include "format.h"
#include "recognize.h"
#include "text_filter.h"

#ifdef WIN32
#  include "window_manager.h"
#  include "window_ptr.h"
#  include "dump_window.h"
#endif

#include "task_executor.h"

#include "application_params.h"

namespace tesseract
{
class TessBaseAPI;
}


namespace tabread
{
namespace console
{

struct Application_context
{
    Application_context();
    Application_context(const Application_context&) = delete;
    Application_context(Application_context&&) = delete;

    Application_context& operator=(const Application_context&) = delete;
    Application_context& operator=(Application_context&&) = delete;

#ifdef WIN32
    w32wrap::Window_manager window_manager;
    w32wrap::Window_ptr widget;
#endif

    std::list<std::string> input_files;
    std::ofstream output;

    cv::Mat current_image;

    imgproc::Image_filter_chain preliminary_filter;
    imgproc::Image_filter_chain pre_ocr_filter;
    imgproc::OCR_rect_filter ocr_rect_filter;

    std::unique_ptr<imgproc::Contour_filter> contour_filter;
    std::unique_ptr<imgproc::Rect_filter> rect_filter;
    std::unique_ptr<imgproc::Rect_bulk_filter> rect_bulk_filter;

    std::unique_ptr<actions::Resource_provider_base<text::Recognize>> ocr_provider;

    std::unique_ptr<tasks::Task_executor> task_executor;

    std::unique_ptr<text::Text_filter> text_filter;
    std::unique_ptr<text::Format> writer;

    std::function<bool(int, const std::vector<std::vector<std::string>>&)> is_more_data_available;
};

std::list<Setup_context_function> make_setup_context(Application_context& context);

void setup_input(Application_context& context, const Application_params& params);
void setup_input_file(Application_context& context, const Application_params& params);
#ifdef WIN32
void setup_input_widget(Application_context& context, const Application_params& params);
#endif
void setup_output_file(Application_context& context, const Application_params& params);
void setup_ocr(Application_context& context, const Application_params& params);
void setup_contour_filters(Application_context& context, const Application_params& params);
void setup_ocr_task_executor(Application_context& context, const Application_params& params);
void setup_formatter(Application_context& context, const Application_params& params);
void setup_text_filter(Application_context& context, const Application_params& params);
void setup_preliminary_filter(Application_context& context, const Application_params& params);
void setup_pre_ocr_filter(Application_context& context, const Application_params& params);
void setup_data_available_func(Application_context& context, const Application_params& params);

bool is_more_data_available_app(int page_num, const std::vector<std::vector<std::string>>& text);
bool is_more_data_available_images(int total, int page_num, const std::vector<std::vector<std::string>>& text);

}
}
