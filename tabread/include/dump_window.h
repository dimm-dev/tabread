#pragma once

#include "image_filter.h"

#include "window_manager.h"
#include "window_ptr.h"

namespace tabread
{
namespace console
{

// TODO: detect window size changing
class Dump_window : public tabread::imgproc::Image_filter
{
    public:
        Dump_window(tabread::w32wrap::Window_ptr window, tabread::w32wrap::Window_manager& wm, cv::Mat& image_buffer);
        void execute(cv::Mat& dst) override;

    private:
        tabread::w32wrap::Rect _window_rect;
        tabread::w32wrap::Window_ptr _window;
        tabread::w32wrap::Window_manager& _wm;
        cv::Mat& _image;
};

}
}
