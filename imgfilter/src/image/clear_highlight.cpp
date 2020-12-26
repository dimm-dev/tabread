#include "image/clear_highlight.h"

namespace
{
bool operator<(const cv::Vec4b, const cv::Vec4b);
}

namespace tabread
{
namespace imgproc
{

const cv::Vec4b Clear_highlight_color = { 215, 120, 0 };
const cv::Vec4b Clear_highlight_alias_threshold = { 10, 10, 10 };
const cv::Vec4b Clear_highlight_tabhead_color = { 240, 240, 240 };
const cv::Vec4b Clear_highlight_target = { 255, 255, 255 };
const cv::Vec4b Clear_highlight_black = { 0, 0, 0 };

void Clear_highlight_filter::execute(cv::Mat& dst)
{
	if (dst.channels() < 4)
		return;
	for (int y = 0; y < dst.rows; ++y)
	{
		int masked = 0;
		for (int x = 0; x < dst.cols; ++x)
		{
			cv::Vec4b& pixel = dst.at<cv::Vec4b>(y, x);
			if (Clear_highlight_color == pixel)
			{
				pixel = Clear_highlight_target;
				masked++;
			}
			else if ((masked > 0) && (Clear_highlight_target == pixel))
				pixel = Clear_highlight_black;
		}
	}
}

void Clear_cells_filter::execute(cv::Mat& dst)
{
	if (dst.channels() < 4)
		return;

	for (int y = 0; y < dst.rows; ++y)
	{
		for (int x = 0; x < dst.cols; ++x)
		{
			cv::Vec4b& pixel = dst.at<cv::Vec4b>(y, x);
			if ((Clear_highlight_color == pixel) || (pixel < Clear_highlight_alias_threshold) || (Clear_highlight_tabhead_color == pixel))
				pixel = Clear_highlight_target;
		}
	}
}

}
}

namespace
{

bool operator<(const cv::Vec4b l, const cv::Vec4b r)
{
	cv::Vec4s ll = l;
	cv::Vec4s rr = r;
	cv::Vec4s d = ll - rr;
	return (d[0] < 0) || (d[0] == 0 && d[1] < 0) || (d[0] == 0 && d[1] == 0 && d[2] < 0);
}

}
