#include "format.h"

namespace tabread
{
namespace text
{

Format::~Format() noexcept
{
}

CSV_format::CSV_format(std::ostream& output) : _output { output }
{
}

void CSV_format::write_field(const std::string& field)
{
    _output << field << ";";
}

void CSV_format::eol()
{
    _output << '\n';
}

}
}
