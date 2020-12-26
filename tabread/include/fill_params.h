#pragma once

#include <list>

#include "application_params.h"

namespace tabread
{
namespace console
{

std::list<Fill_params_function> make_fill_params();

void fill_paths(Application_params& params, const Command_line_params& source);
void fill_thread_type(Application_params& params, const Command_line_params& source);
void fill_input_files(Application_params& params, const Command_line_params& source);
#ifdef WIN32
void fill_source_window(Application_params& params, const Command_line_params& source);
#endif

}
}
