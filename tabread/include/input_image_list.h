#pragma once

#include "image_filter.h"

namespace tabread
{
namespace console
{

// TODO: detect window size changing
class Input_image_list : public tabread::imgproc::Image_filter
{
    public:
        Input_image_list(const std::list<std::string>& input_list, cv::Mat& image_buffer);
        void execute(cv::Mat& dst) override;

    private:
        cv::Mat& _image;
        const std::list<std::string>& _input;
        std::list<std::string>::const_iterator _pos;
};

}
}
