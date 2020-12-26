#pragma once

#include <opencv2/imgcodecs.hpp>

#include "action.h"

namespace tabread
{
namespace imgproc
{

using Image_filter = actions::Action_base<cv::Mat, void>;
using Image_filter_chain = actions::Action_chain<cv::Mat>;

}
}
