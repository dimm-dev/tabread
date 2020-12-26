#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <stack>
#include <stdexcept>

namespace tabread
{
namespace actions
{

struct Dummy_lock
{
    void lock() {}
    void unlock() {}
};

template <typename T>
class Resource_provider_base
{
    public:
        virtual ~Resource_provider_base() noexcept {}

        virtual std::shared_ptr<T> create() = 0;
};

template <typename T, typename Lock>
class Shared_resource : public Resource_provider_base<T>
{
    public:
        Shared_resource(std::function<T*(void)> create_func) : _create { create_func }, _lock_count { 0 }
        {
        }

        // TODO: what if _lock_count != 0?
        ~Shared_resource() noexcept
        {
        }

        //
        // FIXME: user-defined destructor disables default copy/move constructor and = operation
        //

        std::shared_ptr<T> create() override
        {
            const std::lock_guard<Lock> guard(_lock);

            if (!_resource)
                _resource.reset(_create());

            ++_lock_count;

            std::function<void(T*)> deleter = std::bind(&Shared_resource<T, Lock>::release, this, std::placeholders::_1);
            return std::shared_ptr<T> { _resource.get(), deleter };
        }

    private:
        std::unique_ptr<T> _resource;
        std::function<T*(void)> _create;
        Lock _lock;
        int _lock_count;

        void release(T* ptr)
        {
            const std::lock_guard<Lock> guard(_lock);

            if (ptr != _resource.get())
                throw std::runtime_error("Shared_resource: attempt to release invalid pointer");

            if (_lock_count == 0)
                throw std::runtime_error("Shared_resource: double resource free?");

            --_lock_count;
        }
};

template <typename T, typename Lock>
class Copy_resource : public Resource_provider_base<T>
{
    public:
        Copy_resource(std::function<T*(void)> create_func) : _create { create_func }
        {
        }

        // TODO: what if _lock_count != 0?
        ~Copy_resource() noexcept
        {
        }

        //
        // FIXME: user-defined destructor disables default copy/move constructor and = operation
        //

        std::shared_ptr<T> create() override
        {
            const std::lock_guard<Lock> guard(_lock);
            std::unique_ptr<T> result_guard;

            if (_resources.empty())
                result_guard.reset(_create());
            else
            {
                result_guard.reset(_resources.top().release());
                _resources.pop();
            }

            std::function<void(T*)> deleter = std::bind(&Copy_resource<T, Lock>::release, this, std::placeholders::_1);
            std::shared_ptr<T> result { result_guard.get(), deleter };
            result_guard.release();

            return result;
        }

    private:
        std::stack<std::unique_ptr<T>> _resources;
        std::function<T*(void)> _create;
        Lock _lock;

        void release(T* ptr)
        {
            std::unique_ptr<T> res_guard { ptr };
            const std::lock_guard<Lock> guard(_lock);
            _resources.emplace(std::move(res_guard));
        }
};

}
}
