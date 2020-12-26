#pragma once

#include "image_filter.h"

namespace tabread
{
namespace imgproc
{

extern const cv::Vec4b Clear_highlight_color;
extern const cv::Vec4b Clear_highlight_alias_threshold;
extern const cv::Vec4b Clear_highlight_tabhead_color;
extern const cv::Vec4b Clear_highlight_target;

class Clear_highlight_filter : public Image_filter
{
    public:
        void execute(cv::Mat& dst) override;
};

class Clear_cells_filter : public Image_filter
{
    public:
        void execute(cv::Mat& dst) override;
};

}
}
