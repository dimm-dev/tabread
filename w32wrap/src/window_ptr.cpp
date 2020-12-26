#include <functional>

#include "win32_utils.h"

#include "window_ptr.h"

namespace
{

template<typename T, typename Mem>
auto make_update(T* obj, Mem mem_fn)
{
    return std::bind(mem_fn, obj);
}

}

namespace tabread
{
namespace w32wrap
{

Window_ptr::Window_ptr(HWND handle) :
    _handle { handle },
        _class { make_update(this, &Window_ptr::update_class) },
        _title { make_update(this, &Window_ptr::update_title) }
{
}

Window_ptr::Window_ptr(const Window_ptr& win) :
    _handle{ win._handle },
        _class { make_update(this, &Window_ptr::update_class) },
        _title { make_update(this, &Window_ptr::update_title) }
{
}

Window_ptr::Window_ptr(Window_ptr&& src) :
    _handle{ src._handle },
        _class { make_update(this, &Window_ptr::update_class) },
        _title { make_update(this, &Window_ptr::update_title) }
{
    src._handle = nullptr;
}

Window_ptr& Window_ptr::operator=(const Window_ptr& o)
{
    _handle = o._handle;
    return *this;
}

Window_ptr& Window_ptr::operator=(Window_ptr&& src)
{
    _handle = src._handle;
    src._handle = nullptr;
    return *this;
}

Window_ptr::~Window_ptr() noexcept
{
}

HWND Window_ptr::get_handle() const
{
    return _handle;
}

const std::string& Window_ptr::get_class() const
{
    return _class.get_value();
}

const std::string& Window_ptr::get_title() const
{
    return _title.get_value();
}

std::string Window_ptr::update_class() const
{
    return tabread::win32_utils::get_window_class(_handle);
}

std::string Window_ptr::update_title() const
{
    return tabread::win32_utils::get_window_title(_handle);
}

}
}
