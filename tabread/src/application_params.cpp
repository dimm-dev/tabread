#include "application_params.h"

namespace tabread
{
namespace console
{

Command_line_argument_exception::Command_line_argument_exception(const char* parameter, const char* message) : _message(parameter)
{
    // FIXME: no need to use stringstream
    _message += ": ";
    _message += message;
}

const char* Command_line_argument_exception::what() const noexcept
{
    return _message.c_str();
}

Command_line_parse_exception::Command_line_parse_exception(const char* message) : _message(message)
{
}

const char* Command_line_parse_exception::what() const noexcept
{
    return _message.c_str();
}

}
}
