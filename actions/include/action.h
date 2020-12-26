#pragma once

#include <algorithm>
#include <functional>
#include <list>
#include <memory>

namespace tabread
{
namespace actions
{

template <typename T, typename R>
class Action_base
{
    public:
        virtual ~Action_base() noexcept {}
        virtual R execute(T& t) = 0;
};

template <typename T>
using Action_guard = std::shared_ptr<Action_base<T, void>>;

template <typename T>
class Action_chain : public Action_base<T, void>
{
    public:
        // FIXME: do not use rvalue to allow multiple instances of the same action?
        template <typename ... U>
        Action_chain(Action_guard<U> ... args) : _tasks{ std::forward<Action_guard<U>>(args)... }
        { 
        }

        void execute(T& t) override
        {
            auto func_adapter = [ &t ](const Action_guard<T>& g) { g->execute(t); };
            std::for_each(_tasks.begin(), _tasks.end(), func_adapter);
        }

    private:
        std::list<Action_guard<T>> _tasks;
};

}
}
