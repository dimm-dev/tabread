#pragma once

#include <list>

#include "application_params.h"

namespace tabread
{
namespace console
{

std::list<Check_params_function> make_check_params();
void check_output_file_param(Application_params& params);
void check_input_params(Application_params& params);

#ifdef WIN32
void check_window_controls_param(Application_params& params);
#endif

}
}
