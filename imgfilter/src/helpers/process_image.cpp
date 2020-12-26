#include "helpers/process_image.h"

namespace tabread
{
namespace imgproc
{

void find_contours(const cv::Mat& img, Contours& contours)
{
    cv::findContours(img, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
}

void detect_grid(const cv::Mat& image, Rects& grid, Contour_filter& contour_filter, Rect_filter& rect_filter, Rect_bulk_filter& rect_bulk_filter)
{
    Contours contours;
    find_contours(image, contours);

    Rects rects;
    for (const auto& c : contours)
    {
        if (contour_filter.execute(c))
        {
            cv::Rect rect = cv::boundingRect(c);
            if (rect_filter.execute(rect))
                rects.push_back(rect);
        }
    }

    rect_bulk_filter.execute(rects);

    std::copy(std::make_move_iterator(rects.begin()), std::make_move_iterator(rects.end()), std::back_inserter(grid));
}

void get_table_cells(const cv::Mat& image, Table& table, Contour_filter& contour_filter, Rect_filter& rect_filter, Rect_bulk_filter& rect_bulk_filter)
{
    imgproc::Rects grid;
    imgproc::detect_grid(image, grid, contour_filter, rect_filter, rect_bulk_filter);
    imgproc::split_rows(grid, table);
}

bool are_rects_on_same_row(const cv::Rect& l, const cv::Rect& r)
{
	bool same_line = ((l.y >= r.y) && (l.y <= (r.y + r.height / 2))) || ((r.y >= l.y) && (r.y <= (l.y + l.height / 2)));

	return same_line;
}

bool test_intersection_area(const cv::Rect& r, int r_area, const cv::Rect& cur)
{
    cv::Rect i = cur & r;
    int iarea = i.area();
    if (iarea > 0)
    {
        int marea = std::min(cur.area(), r_area);
        return (marea / 4) <= iarea;
    }
    return false;
}

void merge_box(const cv::Rect& r, imgproc::Rects& rects)
{
    auto rArea = r.area();

    auto cmp_area = [rArea, &r](const cv::Rect &cur) -> bool
    {
        return test_intersection_area(r, rArea, cur);
    };

    auto it = std::find_if(rects.begin(), rects.end(), cmp_area);
    if (it == rects.end())
        rects.push_back(r);
    else if (it->area() < rArea)
        *it = r;
}

void split_rows(const Rects& rects, Table& table)
{
    Rects row;
    if (!rects.empty())
    {
        Rects::const_iterator i = rects.begin();
        cv::Rect cur = *i;
        row.push_back(*i);

        for (++i; i != rects.end(); ++i)
        {
            if (!are_rects_on_same_row(cur, *i))
            {
                table.emplace_back(std::move(row));
                cur = *i;
            }
            row.push_back(*i);
        }
    }
    if (row.size())
        table.emplace_back(std::move(row));
}

}
}
