#include <functional>
#include <memory>
#include <type_traits>		

#include "window_rect.h"

#include "win32_utils.h"

namespace
{

template<typename F, class... T>
int get_window_property(F f, T... args)
{
    int length = f(std::forward<T>(args)...);
#if 0
// :KLUDGE who's wrong? MSDN?
    if ((length == 0) && (::GetLastError() != ERROR_SUCCESS))
        throw tabread::win32_utils::window_not_found{};
#endif
    return length;
}

template <typename T, typename D>
typename std::enable_if<
	std::is_pointer<T>::value,
	std::unique_ptr<
		typename std::remove_reference<typename std::remove_pointer<T>::type>::type,
		std::function<void(T)>>>::type
make_w32_resource(T v, D func)
{
	using type = typename std::remove_reference<typename std::remove_pointer<T>::type>::type;
	return std::unique_ptr<type, std::function<void(type*)>> { v, func };
}

}

namespace tabread
{
namespace win32_utils
{

const char* window_not_found::Message = "Specified window not found";

const char* window_not_found::what() const noexcept
{
    return Message;
}

std::string get_window_class(HWND hwnd)
{
    char buf[256];
    get_window_property(::GetClassNameA, hwnd, buf, sizeof(buf) / sizeof(buf[0]));
    std::string cname{buf};
    return cname;
}

std::string get_window_title(HWND hwnd)
{
    int length = get_window_property(::GetWindowTextLength, hwnd);
    std::string title;
    if (length > 0)
    {
        title.resize(length + 1);
        get_window_property(::GetWindowTextA, hwnd, &title[0], length + 1);
    }
    return title;
}

void move_window_on_top(HWND hwnd)
{
	::SetForegroundWindow(hwnd);
	::SendMessage(hwnd, WM_ACTIVATE, 1, 0);
}

void scroll_down_window(HWND hwnd)
{
	::SendMessage(hwnd, WM_KEYDOWN, VK_NEXT, 0);
}

bool enum_windows(WNDENUMPROC lpEnumFunc, LPARAM lParam)
{
    return EnumWindows(lpEnumFunc, lParam);
}

bool enum_children(HWND parent, WNDENUMPROC lpEnumFunc, LPARAM lParam)
{
	return EnumChildWindows(parent, lpEnumFunc, lParam);
}

tabread::w32wrap::Rect get_window_rect(HWND hwnd)
{
	RECT win_rect;
	if (::GetWindowRect(hwnd, &win_rect) == 0)
		throw window_not_found{};
	HWND parent = ::GetParent(hwnd);
	RECT parent_rect;
	if ((parent == nullptr) || (::GetWindowRect(parent, &parent_rect) == 0))
		return make_rect(win_rect);
	auto wrect = make_rect(win_rect);
	auto prect = make_rect(parent_rect);
	wrect.width = std::min(wrect.width, prect.width);
	wrect.height = std::min(wrect.height, prect.height);
	return wrect;
}

int dump_window_content(HWND hwnd, const tabread::w32wrap::Rect& window_rect, char* buffer)
{
	auto release_dc = [ hwnd ](HDC dc) { ::ReleaseDC(hwnd, dc); };
	auto delete_dc = [ ](HDC dc) { ::DeleteDC(dc); };
	auto delete_object = [ ](HGDIOBJ ho) { if (ho != HGDI_ERROR) ::DeleteObject(ho); };

	auto window_dc = make_w32_resource(::GetWindowDC(hwnd), release_dc);
	auto window_compatible_dc = make_w32_resource(::CreateCompatibleDC(window_dc.get()), delete_dc);

	BITMAPINFOHEADER  bi;
	bi.biSize = sizeof(BITMAPINFOHEADER); // http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = window_rect.width;
	bi.biHeight = -window_rect.height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	auto window_bitmap = make_w32_resource(::CreateCompatibleBitmap(window_dc.get(), window_rect.width, window_rect.height), delete_object);

	auto replaced = make_w32_resource(::SelectObject(window_compatible_dc.get(), window_bitmap.get()), delete_object);
	if ((!replaced) || (replaced.get() == HGDI_ERROR))
		throw std::invalid_argument("Provided invalid hwnd or rect");

	if (!::BitBlt(window_compatible_dc.get(), 0, 0, window_rect.width, window_rect.height, window_dc.get(), 0, 0, SRCCOPY))
		throw std::runtime_error("BitBlt");

	BITMAP bmp_dst;
	if (::GetObject(window_bitmap.get(), sizeof(BITMAP), &bmp_dst) == 0)
		throw std::runtime_error("GetObject");

	return ::GetDIBits(window_compatible_dc.get(), window_bitmap.get(), 0, window_rect.height, buffer, (BITMAPINFO *)&bi, DIB_RGB_COLORS);
}

}
}
