#include <filesystem>

#include "check_params.h"

namespace fs = std::filesystem;

namespace tabread
{
namespace console
{

std::list<Check_params_function> make_check_params()
{
    return { check_output_file_param, check_input_params };
}

void check_output_file_param(Application_params& params)
{
    if (params.output_path.empty())
        throw Command_line_argument_exception("Output path", "Mandatory parameter 'output path' is not set.");

    fs::path path { params.output_path };
    if (fs::exists(path) && !fs::is_regular_file(path))
        throw Command_line_argument_exception("Output path", "Can not overwrite 'output path' file.");
}

#ifdef WIN32
void check_window_controls_param(Application_params& params)
{
    if (params.window_class.empty())
        throw Command_line_argument_exception("Window title", "Mandatory parameter 'window class' is not set.");
}
#endif

void check_input_params(Application_params& params)
{
#ifdef WIN32
    if (params.input_path.length() == 0)
        check_window_controls_param(params);
#else
    if (params.input_path.length() == 0)
        throw Command_line_argument_exception("Input path", "Mandatory parameter 'input path' is not set.");
#endif
}

}
}
