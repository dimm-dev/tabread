#include <filesystem>

#include "fill_params.h"

namespace tabread
{
namespace console
{

std::list<Fill_params_function> make_fill_params()
{
#ifdef WIN32
    return { fill_paths, fill_source_window, fill_thread_type, fill_input_files };
#else
    return { fill_paths, fill_thread_type, fill_input_files };
#endif
}

void fill_paths(Application_params& params, const Command_line_params& source)
{
    params.output_path = source.get<std::string>("output");
    params.bin_path = std::filesystem::path{params.argv[0]}.parent_path().string();
}

#ifdef WIN32
void fill_source_window(Application_params& params, const Command_line_params& source)
{
    params.window_title = source.get<std::string>("title");
    params.window_class = source.get<std::string>("class");

    params.child_title = source.get<std::string>("ctitle");
    params.child_class = source.get<std::string>("cclass");
}
#endif

void fill_thread_type(Application_params& params, const Command_line_params& source)
{
    params.threaded = source.has("threaded");
}

void fill_input_files(Application_params& params, const Command_line_params& source)
{
    if (source.has("image"))
        params.input_path = source.get<std::string>("image");
}

}
}
