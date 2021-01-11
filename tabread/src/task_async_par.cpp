#include <algorithm>
#include <execution>

#include "task_async_ocr.h"

namespace tabread
{
namespace tasks
{

void process_task_slices(std::vector<Task_slice>& slices, std::function<void(Task_slice&)> function)
{
    std::for_each(std::execution::par, slices.begin(), slices.end(), function);
}

}
}
