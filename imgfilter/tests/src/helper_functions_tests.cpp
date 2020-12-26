#include <gtest/gtest.h>

#include "helpers/process_image.h"

#if 0
bool test_intersection_area(const cv::Rect& r, int r_area, const cv::Rect& cur);
void merge_box(const cv::Rect& r, tabread::imgproc::Rects& rects);
void split_rows(const Rects& rects, Table& table);
#endif

using namespace tabread::imgproc;

TEST(Imgfilter_helpers, rects_on_same_line_false)
{
    cv::Rect base { 100, 100, 25, 15};

    cv::Rect expected_false[] =
    {
        { 0, 0, 10, 10 }, // upper-left far-far away
        { 90, 90, 15, 10 }, // upper-left, touch
        { 90, 90, 15, 15 }, // upper-left small intersection
        { 100, 115, 15, 15 }, // bottom-left, touch
        { 100, 114, 15, 15 }, // bottom-left, small intersection
        { 125, 90, 10, 10 }, // upper-right, touch
        { 120, 115, 10, 10 }, // upper-right, touch
        { 100, 114, 10, 10 } // upper-right, small intersection
    };

    for (size_t i = 0; i < sizeof(expected_false) / sizeof(expected_false[0]); i++)
        ASSERT_FALSE(are_rects_on_same_row(base, expected_false[i])) << "are_rects_on_same_row() FP#" << i;
}

TEST(Imgfilter_helpers, rects_on_same_line_true)
{
    cv::Rect base { 100, 100, 25, 15};

    cv::Rect expected_true[] =
    {
        { 90, 90, 45, 35 }, // include
        { 101, 101, 20, 14 }, // bound
        { 80, 100, 10, 15 }, // left-of
        { 130, 100, 10, 15 }, // right-of
        { 80, 100, 45, 15 } // large intersection
    };

    for (size_t i = 0; i < sizeof(expected_true) / sizeof(expected_true[0]); i++)
        ASSERT_TRUE(are_rects_on_same_row(base, expected_true[i])) << "are_rects_on_same_row() FN#" << i;
}
