#pragma once

#include "image_filter.h"

namespace tabread
{
namespace imgproc
{

class Convert_gray_filter : public Image_filter
{
    public:
        void execute(cv::Mat& dst) override;
};

}
}
