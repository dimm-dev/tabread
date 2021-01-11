#include "task_async_ocr.h"

namespace tabread
{
namespace tasks
{
    
Task_slice::Task_slice(int rn,
    std::vector<std::string>* text,
        const imgproc::Rects* r) :
    row_num{ rn },
        rects{ r },
        row_text{ text },
        failed{ false }
{
}

}
}
