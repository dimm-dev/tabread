#include "helpers/process_image.h"

#include "contour/contour_area_filter.h"

namespace tabread
{
namespace imgproc
{

const Contour_area_filter_params Contour_area_filter_params_value = { 25, 10000 };

bool Contour_area_filter::execute(const Contour& c)
{
    auto area = cv::contourArea(c);
    return area >= Contour_area_filter_params_value.min_area &&
        area <= Contour_area_filter_params_value.max_area;
}

const Rect_area_filter_params Rect_area_filter_params_value { 12, 900, 10, 30 };

bool Rect_area_filter::execute(const cv::Rect& r)
{
    return r.width > r.height &&
        r.width > Rect_area_filter_params_value.min_width &&
        r.height > Rect_area_filter_params_value.min_height &&
        r.width < Rect_area_filter_params_value.max_width &&
        r.height < Rect_area_filter_params_value.max_height;
}

void Merge_rects_filter::execute(Rects& rects)
{
	Rects result;

	for (const auto& r : rects)
		merge_box(r, result);

	std::swap(rects, result);
}

void Sort_rects::execute(Rects& rects)
{
	auto cmp = [](const cv::Rect l, const cv::Rect& r) -> bool
	{
		bool same_row = are_rects_on_same_row(l, r);
		if (same_row)
			return l.x < r.x;
		return l.y < r.y;
	};

	std::sort(rects.begin(), rects.end(), cmp);
}

}
}
