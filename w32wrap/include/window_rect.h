#pragma once

namespace tabread
{
namespace w32wrap
{

struct Rect
{
    int x;
    int y;
    int width;
    int height;

    Rect(int xx = 0, int yy = 0, int w = 0, int h = 0) : x{xx}, y{yy}, width{w}, height{h}
    {
    }

    bool operator==(const Rect& other)
    {
        return x == other.x && y == other.y && width == other.width && height == other.height;
    }

    bool operator!=(const Rect& other)
    {
        return !(*this == other);
    }
};

}
}
