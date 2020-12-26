#pragma once
#include <vector>

#include <opencv2/imgproc.hpp>

#include "action.h"

namespace tabread
{
namespace imgproc
{

class Contours_filter;
using Contour = std::vector<cv::Point>;
using Contours = std::vector<Contour>;
using Rects = std::vector<cv::Rect>;
using Contour_filters = std::vector<std::shared_ptr<Contours_filter>>;

using Table = std::vector<Rects>;

using Contour_filter = actions::Action_base<const Contour, bool>;
using Rect_filter = actions::Action_base<const cv::Rect, bool>;

using Rect_bulk_filter = actions::Action_base<Rects, void>;
using Rect_bulk_filter_chain = actions::Action_chain<Rects>;

}
}
