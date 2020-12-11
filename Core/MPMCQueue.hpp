/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: MPMC Queue
 */

#pragma once

#include <atomic>
#include <cstdlib>

#include "Utils.hpp"

namespace Core
{
    template<typename Type>
    class MPMCQueue;
}

/**
 * @brief The MPMC queue is a lock-free queue that only supports a Single Producer but Multiple Consumers
 *
 * @tparam Type to be inserted
 */
template<typename Type>
class alignas_double_cacheline Core::MPMCQueue
{
public:
    /** @brief Each cell represent the queued type and a sequence index */
    struct Cell
    {
        std::atomic<std::size_t> sequence { 0 };
        Type data;
    };

    /** @brief Buffer structure containing all cells */
    struct Buffer
    {
        std::size_t mask { 0 };
        Cell *data { nullptr };
    };

    /** @brief Cache of producers or consumers */
    struct Cache
    {
        Buffer buffer;
    };

    /** @brief Default constructor initialize the queue */
    MPMCQueue(const std::size_t size);

    /** @brief Destruct and release all memory (unsafe) */
    ~MPMCQueue(void) noexcept_destructible(Type);

    /** @brief Return the size of the queue */
    [[nodiscard]] std::size_t size(void) const noexcept;

    /** @brief Push a single element into the queue
     *  @return true if the element has been inserted */
    template<bool MoveOnSuccess = false, typename ...Args>
    [[nodiscard]] std::enable_if_t<std::is_constructible_v<Type, Args...>, bool>
            push(Args &&...args)
        noexcept_constructible(Type, Args...);

    /** @brief Pop a single element from the queue
     *  @return true if an element has been extracted */
    [[nodiscard]] bool pop(Type &value)
        noexcept(nothrow_destructible(Type) && nothrow_forward_constructible(Type));

    /** @brief Clear all elements of the queue (unsafe) */
    void clear(void) noexcept_destructible(Type) { for (Type tmp; pop(tmp);); }

private:
    alignas_cacheline std::atomic<std::size_t> _tail { 0 }; // Tail accessed by producers
    alignas_cacheline Cache _tailCache; // Cache accessed by producers
    alignas_cacheline std::atomic<std::size_t> _head { 0 }; // Head accessed by consumers
    alignas_cacheline Cache _headCache; // Cache accessed by consumers

    /** @brief Copy and move constructors disabled */
    MPMCQueue(const MPMCQueue &other) = delete;
    MPMCQueue(MPMCQueue &&other) = delete;
};

static_assert_sizeof(Core::MPMCQueue<int>, 4 * Core::CacheLineSize);
static_assert_alignof_double_cacheline(Core::MPMCQueue<int>);

#include "MPMCQueue.ipp"