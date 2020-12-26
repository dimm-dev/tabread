#pragma once

#include <list>
#include <memory>

#include "contour_filter.h"

// TODO: rearrange filters over files and folders

namespace tabread
{
namespace imgproc
{

struct Contour_area_filter_params
{
    int min_area;
    int max_area;
};

extern const Contour_area_filter_params Contour_area_filter_params_value;

class Contour_area_filter : public Contour_filter
{
    public:
        bool execute(const Contour&) override;
};

struct Rect_area_filter_params
{
    int min_width;
    int max_width;
    int min_height;
    int max_height;
};

extern const Rect_area_filter_params Rect_area_filter_params_value;

class Rect_area_filter : public Rect_filter
{
    public:
        bool execute(const cv::Rect&) override;
};

class Merge_rects_filter : public Rect_bulk_filter
{
    public:
        void execute(Rects&) override;
};

class Sort_rects : public Rect_bulk_filter
{
    public:
        void execute(Rects&) override;
};

}
}
