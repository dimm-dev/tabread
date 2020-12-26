#include <Windows.h>

#include <iostream>
#include <memory>

#include "win32_utils.h"
#include "window_manager.h"

HWND reload_console();
void resize_console(HWND hwnd, int x, int y, int w, int h);

static const int console_off_x = 100;
static const int console_off_y = 100;

static const int console_width = 200;
static const int console_height = 200;

int main(int, char**)
{
    try
    {
        HWND hwnd = reload_console();
        resize_console(hwnd, console_off_x, console_off_y, console_width, console_height);
        tabread::w32wrap::Window_manager manager;
        auto windows { manager.get_windows() };
        for (auto& w : windows)
            std::cout << w.get_handle() << ";" << w.get_class() << ";" << w.get_title() << '\n';

        auto pred = [ hwnd ](const tabread::w32wrap::Window_ptr& w) { return w.get_handle() == hwnd; };

        auto filterred { manager.get_windows(pred) };
        if (filterred.size() != 1)
            throw std::runtime_error("filter doesn't work");

        tabread::w32wrap::Rect expected { console_off_x, console_off_y, console_width, console_height };
        tabread::w32wrap::Rect r = tabread::win32_utils::get_window_rect(hwnd);
        if (r != expected)
            throw std::runtime_error("unexpected rect");
    
        std::unique_ptr<char> img_buffer { new char[r.width * r.height * 4] };
        tabread::win32_utils::dump_window_content(hwnd, r, img_buffer.get());

        std::cout << r.x << " " << r.y << " " << r.width << " " << r.height << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}

HWND reload_console()
{
    HWND console = ::GetConsoleWindow();

    if (console)
        ::FreeConsole();

    ::AllocConsole();
    console = ::GetConsoleWindow();

    // FIXME: switched to the console window handler?
    // ::SetConsoleTitle("w32test.exe");

    return console;
}

void resize_console(HWND hwnd, int x, int y, int w, int h)
{
    ::SetWindowPos(hwnd, HWND_TOPMOST, x, y, w, h, SWP_SHOWWINDOW);
}
