#include <algorithm>
#include <cmath>
#include <future>
#include <thread>

#include "task_async_ocr.h"

namespace
{
template <typename It>
void process_tasks_slice(It start, It end, std::function<void(tabread::tasks::Task_slice &)> function);
}

namespace tabread
{
namespace tasks
{

void process_task_slices(std::vector<Task_slice>& slices, std::function<void(Task_slice&)> function)
{
    int tasks = std::min(static_cast<size_t>(std::thread::hardware_concurrency()), slices.size());
    int rows_per_task = slices.size() / tasks;
    int shift = slices.size() % tasks;
    std::vector<std::future<void>> futures;
    futures.reserve(tasks);
    for (auto i = slices.begin(); i != slices.end();)
    {
        auto e = i;
        std::advance(i, rows_per_task);
        if (shift > 0)
        {
            shift--;
            std::advance(i, 1);
        }
        futures.emplace_back(std::move(std::async(std::launch::async, process_tasks_slice<decltype(slices.begin())>, e, i, function)));
    }
    std::for_each(futures.begin(), futures.end(), [](auto& f) { f.wait(); });
}

}
}

namespace
{

template <typename It>
void process_tasks_slice(It start, It end, std::function<void(tabread::tasks::Task_slice &)> function)
{
    std::for_each(start, end, function);
}

}
