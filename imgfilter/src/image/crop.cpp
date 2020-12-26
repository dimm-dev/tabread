#include <opencv2/imgproc.hpp>

#include "image/crop.h"

using namespace cv;

namespace tabread
{
namespace imgproc
{

const cv::Rect Crop_params_value = { 38, 72, 30, 75 };

void Crop_filter::execute(cv::Mat& dst)
{
    cv::Rect roi
    {
        .x = 0 + Crop_params_value.x,
        .y = 0 + Crop_params_value.y,
        .width = dst.cols - Crop_params_value.width - Crop_params_value.x,
        .height = dst.rows - Crop_params_value.height - Crop_params_value.y
    };

    dst = dst(roi);
}

}
}
