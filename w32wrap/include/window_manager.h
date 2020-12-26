#pragma once

#include <functional>
#include <vector>

#include "window_ptr.h"
#include "window_rect.h"

namespace tabread
{
namespace w32wrap
{

class Window_manager
{
    public:
        using window_predicate = std::function<bool(const Window_ptr&)>;
        static bool all(const Window_ptr& ) { return true; }

        std::vector<Window_ptr> get_windows(window_predicate predicate = all);
        std::vector<Window_ptr> get_window_children(const Window_ptr& parent, window_predicate predicate = all);

        void move_on_top(const Window_ptr& window);
        void scroll_down(const Window_ptr& window);
        Rect get_window_rect(const Window_ptr& window);
        void get_window_content(const Window_ptr& window, const Rect& window_rect, char* buffer);
};

}
}
