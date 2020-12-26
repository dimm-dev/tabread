#pragma once

#include <functional>

namespace tabread
{
namespace w32wrap
{

template <typename T>
class lazy_value
{
    public:
        using update_function_type = std::function<T(void)>;

    private:
        mutable T _value;
        mutable bool _initialized = false;
        update_function_type _update_function;

    public:
        lazy_value(update_function_type update_func) : _update_function(update_func) {}

        lazy_value(const lazy_value<T>&) = delete;

        lazy_value(lazy_value<T>&& nv)
        {
            std::swap(_value, nv._value);
            std::swap(_update_function, nv._update_function);
            std::swap(_initialized, nv._initialized);
        }

        lazy_value<T>& operator=(const lazy_value<T>&) = delete;

        lazy_value<T>& operator=(lazy_value<T>&& nv)
        {
            std::swap(_value, nv._value);
            std::swap(_update_function, nv._update_function);
            std::swap(_initialized, nv._initialized);
            return *this;
        }

        const std::string& get_value() const
        {
            if (!_initialized)
            {
                _value = std::move(_update_function());
                _initialized = true;
            }

            return _value;
        }
};

}
}
