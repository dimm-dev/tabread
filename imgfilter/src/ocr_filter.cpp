#include "ocr_filter.h"

namespace tabread
{
namespace imgproc
{

void OCR_rect_filter::execute(const cv::Mat& src, const cv::Rect& roi, cv::Mat& dst)
{
    cv::Mat tmp;
    cv::Rect r{ roi.x + 1, roi.y + 1, roi.width - 2, roi.height - 2 };
    cv::resize(src(r), tmp, cv::Size(), 3, 3, cv::INTER_CUBIC);
    cv::bilateralFilter(tmp, dst, 1, 3, 3);
}

}
}
