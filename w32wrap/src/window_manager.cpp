#include <iostream>

#include "win32_utils.h"

#include "window_manager.h"

namespace
{

struct Enum_context
{
    using predicate = tabread::w32wrap::Window_manager::window_predicate;
    predicate& pred;
    std::vector<tabread::w32wrap::Window_ptr> windows;
    explicit Enum_context(predicate& p) : pred(p) {}
};

BOOL CALLBACK enum_callback(HWND hwnd, LPARAM arg);

}

namespace tabread
{
namespace w32wrap
{

std::vector<Window_ptr> Window_manager::get_windows(Window_manager::window_predicate predicate)
{
    Enum_context ctx { predicate };
    tabread::win32_utils::enum_windows(enum_callback, reinterpret_cast<LPARAM>(&ctx));
    return ctx.windows;
}

std::vector<Window_ptr> Window_manager::get_window_children(const Window_ptr& parent, Window_manager::window_predicate predicate)
{
    Enum_context ctx { predicate };
    tabread::win32_utils::enum_children(parent.get_handle(), enum_callback, reinterpret_cast<LPARAM>(&ctx));
    return ctx.windows;
}

void Window_manager::move_on_top(const Window_ptr& window)
{
    tabread::win32_utils::move_window_on_top(window.get_handle());
}

void Window_manager::scroll_down(const Window_ptr& window)
{
    tabread::win32_utils::scroll_down_window(window.get_handle());
}

Rect Window_manager::get_window_rect(const Window_ptr& window)
{
    return tabread::win32_utils::get_window_rect(window.get_handle());
}

void Window_manager::get_window_content(const Window_ptr& window, const Rect& window_rect, char* buffer)
{
    tabread::win32_utils::dump_window_content(window.get_handle(), window_rect, buffer);
}

}
}

namespace
{

BOOL CALLBACK enum_callback(HWND hwnd, LPARAM arg)
{
    Enum_context* ctx = reinterpret_cast<Enum_context*>(arg);
    try
    {
        tabread::w32wrap::Window_ptr window(hwnd);
        if (ctx->pred(window))
            ctx->windows.emplace_back(window);
    }
    catch(const tabread::win32_utils::window_not_found&)
    {
        // :KLUDGE: ignoring ?
    }
    catch (...)
    {
        return false;
    }

    return true;
}

}
