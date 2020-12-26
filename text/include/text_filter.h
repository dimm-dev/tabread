#pragma once

#include <map>
#include <string>

#include "action.h"

namespace tabread
{
namespace text
{

using Text_filter = actions::Action_base<std::string, void>;

class Dictionary_subst_filter : public Text_filter
{
    public:
        using Dict = std::map<std::string, std::string>;
        explicit Dictionary_subst_filter(const Dict& dict);
        explicit Dictionary_subst_filter(Dict&& dict);

        void execute(std::string& src) override;

    private:
        Dict _dict;
};

}
}
