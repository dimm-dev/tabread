#pragma once

#include <Windows.h>

#include <stdexcept>
#include <string>

#include "window_rect.h"

// TODO: use opencv rect?

namespace tabread
{
namespace win32_utils
{

static inline tabread::w32wrap::Rect make_rect(RECT& r)
{
    return tabread::w32wrap::Rect(0, 0, r.right - r.left, r.bottom - r.top);
}

class window_not_found : public std::exception
{
    private:
        static const char* Message;

    public:
        const char* what() const noexcept override;
};

std::string get_window_class(HWND hwnd);
std::string get_window_title(HWND hwnd);

void move_window_on_top(HWND hwnd);
void scroll_down_window(HWND hwnd);

bool enum_windows(WNDENUMPROC lpEnumFunc, LPARAM lParam);
bool enum_children(HWND parent, WNDENUMPROC lpEnumFunc, LPARAM lParam);

tabread::w32wrap::Rect get_window_rect(HWND hwnd);
int dump_window_content(HWND hwnd, const tabread::w32wrap::Rect& window_rect, char* buffer);

}
}
