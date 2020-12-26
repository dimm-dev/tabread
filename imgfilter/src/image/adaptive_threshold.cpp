#include <opencv2/imgproc.hpp>

#include "image/adaptive_threshold.h"

namespace tabread
{
namespace imgproc
{

const Theshold_params Theshold_params_default = { 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, 1, 3, 0 };


Adaptive_threshold_filter::Adaptive_threshold_filter(const Theshold_params& params) : _params { params }
{
}

void Adaptive_threshold_filter::execute(cv::Mat& dst)
{
    cv::Mat src = dst.clone();
    cv::adaptiveThreshold(src, dst, _params.max_value, _params.method, _params.type, _params.block_size, _params.substract_c);
}

}
}
