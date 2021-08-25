/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: PMR alias namespace
 */

#pragma once

#if __APPLE__
# include <experimental/memory_resource>
# include <Core/Utils.hpp>

namespace std::pmr
{
    using namespace std::experimental::pmr;

    class unsynchronized_pool_resource : public memory_resource
    {
    public:
        virtual ~unsynchronized_pool_resource(void) = default;

    private:
        virtual void* do_allocate(std::size_t bytes, std::size_t alignment) { return Core::Utils::AlignedAlloc(bytes, alignment); }
        virtual void do_deallocate(void *ptr, std::size_t, std::size_t) { return Core::Utils::AlignedFree(ptr); }
        virtual bool do_is_equal(memory_resource const &) const noexcept { return false; }
    };

    class synchronized_pool_resource : public memory_resource
    {
    public:
        virtual ~synchronized_pool_resource(void) = default;

    private:
        virtual void* do_allocate(std::size_t bytes, std::size_t alignment) { return Core::Utils::AlignedAlloc(bytes, alignment); }
        virtual void do_deallocate(void *ptr, std::size_t, std::size_t) { return Core::Utils::AlignedFree(ptr); }
        virtual bool do_is_equal(memory_resource const &) const noexcept { return false; }
    };
}

#else
# include <memory_resource>
#endif