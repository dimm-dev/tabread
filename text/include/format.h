#pragma once

#include <algorithm>
#include <iterator>
#include <ostream>
#include <string>

namespace tabread
{
namespace text
{

// TODO: add iterator adapter
class Format
{
    public:
        virtual ~Format() noexcept;

        virtual void write_field(const std::string& field) = 0;
        virtual void eol() = 0;
};

class CSV_format : public Format
{
    public:
        CSV_format(std::ostream& output);

        void write_field(const std::string& field) override;
        void eol() override;

    private:
        std::ostream& _output;
};

template <typename It>
void format_row(It begin, It end, Format& f)
{
    std::for_each(begin, end, [ &f ](typename It::value_type i) { f.write_field(i); });
    f.eol();
}

template <typename It>
void format_table(It begin, It end, Format& f)
{
    std::for_each(begin, end, [ &f ](typename It::value_type i) { format_row(i.begin(), i.end(), f); });
}

}
}
