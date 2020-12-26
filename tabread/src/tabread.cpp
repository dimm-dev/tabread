#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>

#include <opencv2/highgui.hpp>

#include "application_context.h"
#include "application_params.h"
#include "check_params.h"
#include "fill_params.h"

#ifdef WIN32
#  include "win32_utils.h"
#endif

#include "helpers/process_image.h"

#include "contour_filter.h"
#include "image_filter.h"

bool initialize(tabread::console::Application_context& context, int argc, char** argv);
void run_cycle(tabread::console::Application_context& context);

int main(int argc, char* argv[])
{
    tabread::console::Application_context context;
    try
    {
        if (initialize(context, argc, argv))
            run_cycle(context);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}

bool initialize(tabread::console::Application_context& context, int argc, char** argv)
{
    return tabread::console::initialize(
        tabread::console::make_fill_params(),
        tabread::console::make_check_params(),
        tabread::console::make_setup_context(context),
        argc, argv);
}

tabread::imgproc::Table detect_image_table(tabread::imgproc::Image_filter& image_filter,
    tabread::imgproc::Contour_filter& contour_filter,
    tabread::imgproc::Rect_filter& rect_filter,
    tabread::imgproc::Rect_bulk_filter& rect_bulk_filter)

{
    cv::Mat prepared_image;
    image_filter.execute(prepared_image);
    tabread::imgproc::Table table;
    tabread::imgproc::get_table_cells(prepared_image, table, contour_filter, rect_filter, rect_bulk_filter);

    return table;
}

std::vector<std::vector<std::string>> get_image_text(const cv::Mat& src,
    const tabread::imgproc::Table& table,
    tabread::imgproc::Image_filter& pre_ocr_filter,
    tabread::tasks::Task_executor& ocr_task_executor)
{
    cv::Mat image = src.clone();

    pre_ocr_filter.execute(image);
    auto text = ocr_task_executor.execute(image, table);

    return text;
}

void run_cycle(tabread::console::Application_context& context)
{
    int page_index = 0;
    std::vector<std::vector<std::string>> text;
    do
    {
        auto table = detect_image_table(context.preliminary_filter,
            *context.contour_filter,
            *context.rect_filter,
            *context.rect_bulk_filter);

        text = std::move(
            get_image_text(context.current_image,
                table,
                context.pre_ocr_filter,
                *context.task_executor));

        tabread::text::format_table(text.begin(), text.end(), *context.writer);
    }
    while (context.is_more_data_available(++page_index, text));
}
