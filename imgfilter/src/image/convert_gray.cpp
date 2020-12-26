#include <opencv2/imgproc.hpp>

#include "image/convert_gray.h"

namespace tabread
{
namespace imgproc
{

void Convert_gray_filter::execute(cv::Mat& dst)
{
    cv::Mat src = dst.clone();
    cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
}

}
}
