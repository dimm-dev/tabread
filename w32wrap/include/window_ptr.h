#pragma once

#include <Windows.h>

#include <string>

#include "lazy_value.h"

namespace tabread
{
namespace w32wrap
{

class Window_ptr
{
    private:
        HWND _handle;
        lazy_value<std::string> _class;
        lazy_value<std::string> _title;

    public:
        explicit Window_ptr(HWND handle = nullptr);
        Window_ptr(const Window_ptr& win);
        Window_ptr(Window_ptr&& win);
        ~Window_ptr() noexcept;

        Window_ptr& operator=(const Window_ptr&);
        Window_ptr& operator=(Window_ptr&&);

        bool operator==(const Window_ptr& other) const;

        HWND get_handle() const;

        const std::string& get_class() const;
        const std::string& get_title() const;

    private:
        std::string update_class() const;
        std::string update_title() const;
};

}
}
