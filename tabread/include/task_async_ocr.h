#pragma once

#include <functional>
#include <vector>

#include "contour_filter.h"

namespace tabread
{
namespace tasks
{

struct Task_slice
{
    int row_num;
    const imgproc::Rects *rects;
    std::vector<std::string> *row_text;

    bool failed;

    Task_slice(int rn = 0, std::vector<std::string> *text = nullptr, const imgproc::Rects *r = nullptr);
};

void process_task_slices(std::vector<Task_slice>& slices, std::function<void(Task_slice&)> function);

}
}
