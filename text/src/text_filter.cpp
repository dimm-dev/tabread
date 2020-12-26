#include "text_filter.h"

namespace tabread
{
namespace text
{

using Dict = std::map<std::string, std::string>;

Dictionary_subst_filter::Dictionary_subst_filter(const Dict& dict) : _dict{ dict }
{
}

Dictionary_subst_filter::Dictionary_subst_filter(Dict&& dict) : _dict { dict }
{
}

void Dictionary_subst_filter::execute(std::string& src)
{
	for (const auto& s : _dict)
	{
        std::string::size_type pos = 0;
        while ((pos = src.find(s.first, pos)) != std::string::npos)
		    src.replace(pos, s.first.length(), s.second);
	}
}

}
}
