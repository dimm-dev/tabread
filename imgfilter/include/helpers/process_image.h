#pragma once

#include "contour_filter.h"

// TODO: rename
namespace tabread
{
namespace imgproc
{

void find_contours(const cv::Mat& img, Contours& contours);
void detect_grid(const cv::Mat& image, Rects& grid, Contour_filter& contour_filter, Rect_filter& rect_filter, Rect_bulk_filter& rect_bulk_filter);
void get_table_cells(const cv::Mat& image, Table& table, Contour_filter& contour_filter, Rect_filter& rect_filter, Rect_bulk_filter& rect_bulk_filter);

bool are_rects_on_same_row(const cv::Rect& l, const cv::Rect& r);
bool test_intersection_area(const cv::Rect& r, int r_area, const cv::Rect& cur);
void merge_box(const cv::Rect& r, tabread::imgproc::Rects& rects);
void split_rows(const Rects& rects, Table& table);

}
}
