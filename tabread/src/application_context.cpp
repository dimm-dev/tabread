#include <algorithm>

#include "contour/contour_area_filter.h"
#include "image/adaptive_threshold.h"
#include "image/clear_highlight.h"
#include "image/convert_gray.h"
#include "image/crop.h"
#include "helpers/process_image.h"
#include "contour_filter.h"
#include "image_filter.h"

#include "input_image_list.h"

#include "application_context.h"

namespace
{

template <typename T, typename F>
auto make_caller_wrapper(T& context, F f)
{
    return [ &context, f ](const tabread::console::Application_params& p) { return f(context, p); };
}

}

namespace tabread
{
namespace console
{

Application_context::Application_context()
{
}

std::list<Setup_context_function> make_setup_context(Application_context& context)
{
    return
    {
        make_caller_wrapper(context, setup_input),
        make_caller_wrapper(context, setup_preliminary_filter),
        make_caller_wrapper(context, setup_pre_ocr_filter),
        make_caller_wrapper(context, setup_ocr),
        make_caller_wrapper(context, setup_output_file),
        make_caller_wrapper(context, setup_contour_filters),
        make_caller_wrapper(context, setup_text_filter),
        make_caller_wrapper(context, setup_ocr_task_executor),
        make_caller_wrapper(context, setup_formatter),
        make_caller_wrapper(context, setup_data_available_func)
    };
}

#ifdef WIN32
void setup_input_widget(Application_context& context, const Application_params& params)
{
    auto window_cmp = [ &params ](const tabread::w32wrap::Window_ptr& w)
    {
        // TODO: regexp?
        return  w.get_class().find(params.window_class) != std::string::npos &&
            (params.window_title.empty() || w.get_title().find(params.window_title) != std::string::npos);
    };
    auto found_windows { context.window_manager.get_windows(window_cmp) };
    decltype(found_windows) candidates;

    if (params.child_class.empty() && params.child_title.empty())
    {
        if (found_windows.size() > 0)
            candidates.push_back(found_windows[0]);
    }
    else
    {
        auto child_cmp = [ &params ](const tabread::w32wrap::Window_ptr& c)
        {
            return (params.child_class.empty() || c.get_class().find(params.child_class) != std::string::npos) &&
             (params.child_title.empty() || c.get_title().find(params.child_title) != std::string::npos);
             
        };
        for (auto& w : found_windows)
        {
            auto children { context.window_manager.get_window_children(w, child_cmp) };
            std::copy(children.begin(), children.end(), std::back_inserter(candidates));
        }
    }

    if (candidates.empty())
        throw std::runtime_error("The requested window is not found.");

    context.widget = candidates[0];
}
#endif

void setup_output_file(Application_context& context, const Application_params& params)
{
    context.output.open(params.output_path, std::ofstream::out | std::ofstream::trunc);
    if (!context.output)
        throw std::runtime_error("Unable to create output file.");
}

void setup_ocr(Application_context& context, const Application_params& params)
{
    using Shared_ocr = tabread::actions::Shared_resource<tabread::text::Recognize, tabread::actions::Dummy_lock>;
    using Copy_ocr = tabread::actions::Copy_resource<tabread::text::Recognize, std::mutex>;
#ifdef WIN32
    tabread::text::Recognize::initialize(params.bin_path);
#endif

    auto initialize_func = []() { return new tabread::text::Recognize{}; };
    if (params.threaded)
        context.ocr_provider.reset(new Copy_ocr{ initialize_func });
    else
        context.ocr_provider.reset(new Shared_ocr{ initialize_func });
}

void setup_contour_filters(Application_context& context, const Application_params&)
{
    auto bulk = std::unique_ptr<tabread::imgproc::Rect_bulk_filter>
    {
        new tabread::imgproc::Rect_bulk_filter_chain
        {
            std::shared_ptr<tabread::imgproc::Rect_bulk_filter>{ new tabread::imgproc::Merge_rects_filter{} },
            std::shared_ptr<tabread::imgproc::Rect_bulk_filter>{ new tabread::imgproc::Sort_rects{} }
        }
    };
    context.rect_filter.reset(new tabread::imgproc::Rect_area_filter{});
    context.contour_filter.reset(new tabread::imgproc::Contour_area_filter{});
    context.rect_bulk_filter.reset(bulk.release());
}

void setup_ocr_task_executor(Application_context& context, const Application_params& params)
{
    if (params.threaded)
        context.task_executor.reset(new tabread::tasks::Async_task_executor{ context.ocr_rect_filter, *context.text_filter, *context.ocr_provider });
    else
        context.task_executor.reset(new tabread::tasks::Sequental_task_executor{ context.ocr_rect_filter, *context.text_filter, context.ocr_provider->create() });
}

void setup_text_filter(Application_context& context, const Application_params&)
{
    context.text_filter.reset(new tabread::text::Dictionary_subst_filter
    {{
        { "Ve", "Vx" },
		{ "\\Му", "Vу" },
		{ "Мх=", "Vx=" },
        { ";", " " },
        { "\n", "" }
    }});
}

void setup_formatter(Application_context& context, const Application_params&)
{
    context.writer.reset(new tabread::text::CSV_format{ context.output });
}

// TODO: read list definition from the configuration file
void setup_preliminary_filter(Application_context& context, const Application_params&)
{
    std::shared_ptr<imgproc::Image_filter> input;
#ifdef WIN32
    if (context.input_files.empty())
        input.reset(new tabread::console::Dump_window{context.widget, context.window_manager, context.current_image});
    else
        input.reset(new tabread::console::Input_image_list{ context.input_files, context.current_image });
#else
    input.reset(new tabread::console::Input_image_list{ context.input_files, context.current_image });
#endif
    context.preliminary_filter = tabread::imgproc::Image_filter_chain 
    {
        std::move(input),
#if 0
        std::shared_ptr<tabread::imgproc::Image_filter>{ new tabread::imgproc::Crop_filter{} },
#endif
        std::shared_ptr<tabread::imgproc::Image_filter>{ new tabread::imgproc::Clear_cells_filter{} },
        std::shared_ptr<tabread::imgproc::Image_filter>{ new tabread::imgproc::Convert_gray_filter{} },
        std::shared_ptr<tabread::imgproc::Image_filter>{ new tabread::imgproc::Adaptive_threshold_filter{} }            
    };
}

void setup_pre_ocr_filter(Application_context& context, const Application_params&)
{
    context.pre_ocr_filter = tabread::imgproc::Image_filter_chain
    {
#if 0
        std::shared_ptr<tabread::imgproc::Image_filter>{ new tabread::imgproc::Crop_filter{} },
#endif
        std::shared_ptr<tabread::imgproc::Image_filter>{ new tabread::imgproc::Clear_highlight_filter{} },
        std::shared_ptr<tabread::imgproc::Image_filter>{ new tabread::imgproc::Convert_gray_filter{} }
    };
}

void setup_input(Application_context& context, const Application_params& params)
{
    if (!params.input_path.empty())
        setup_input_file(context, params);
#ifdef WIN32
    else
        setup_input_widget(context, params);
#endif
}

void setup_input_file(Application_context& context, const Application_params& params)
{
    std::istringstream ss(params.input_path);
    std::string token;
    while (std::getline(ss, token, ','))
        context.input_files.emplace_back(token);
}

void setup_data_available_func(Application_context& context, const Application_params&)
{
    if (context.input_files.size() > 0)
        context.is_more_data_available = std::bind(is_more_data_available_images, context.input_files.size(), std::placeholders::_1, std::placeholders::_2);
    else
        context.is_more_data_available = is_more_data_available_app;
}

bool is_row_with_text(const std::vector<std::string>& row)
{
    return std::count_if(row.begin(), row.end(), [](const std::string& t) { return t.size() != 0; }) != 0;
}

bool is_more_data_available_app(int, const std::vector<std::vector<std::string>>& grid)
{
    return grid.size() > 0 && is_row_with_text(grid[grid.size() - 1]);
}

bool is_more_data_available_images(int total_pages, int page_index, const std::vector<std::vector<std::string>>&)
{
    return page_index < total_pages;
}

}
}
