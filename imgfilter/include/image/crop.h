#pragma once

#include "image_filter.h"

namespace tabread
{
namespace imgproc
{

extern const cv::Rect Crop_params_value;

// TODO: check if ROI less than window
class Crop_filter : public Image_filter
{
    public:
        void execute(cv::Mat& dst) override;
};

}
}
