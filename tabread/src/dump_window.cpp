#include "dump_window.h"

namespace tabread
{
namespace console
{

Dump_window::Dump_window(tabread::w32wrap::Window_ptr window, tabread::w32wrap::Window_manager& wm, cv::Mat& image) :
    _window_rect { wm.get_window_rect(window) },
        _window(window),
        _wm(wm),
        _image(image)
{
    _image.create(_window_rect.height, _window_rect.width, CV_8UC4);
}

void Dump_window::execute(cv::Mat& dst)
{
    _wm.move_on_top(_window);
    _wm.get_window_content(_window, _window_rect, reinterpret_cast<char*>(_image.data));
    dst = _image.clone();
    _wm.scroll_down(_window);
}

}
}
