#include <opencv2/imgproc.hpp>

#include "input_image_list.h"

namespace tabread
{
namespace console
{

Input_image_list::Input_image_list(const std::list<std::string>& input, cv::Mat& image) :
    _image(image),
        _input(input),
        _pos { _input.begin() }
{
}

void Input_image_list::execute(cv::Mat& dst)
{
    if (_pos != _input.end())
    {
        _image = cv::imread(*_pos);
        if (_image.data == nullptr)
            throw std::runtime_error("Input_image_list: input file not found or is not an image.");
        dst = _image.clone();
        ++_pos;
    }
    else
    {
        dst.create(256, 256, CV_8UC4);
        dst.setTo(cv::Scalar(255, 255, 255));
    }
}

}
}
