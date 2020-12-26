#pragma once

#include <opencv2/imgproc.hpp>

namespace tabread
{
namespace imgproc
{

class OCR_rect_filter
{
    public:
        void execute(const cv::Mat& src, const cv::Rect& roi, cv::Mat& dst);
};

}
}
