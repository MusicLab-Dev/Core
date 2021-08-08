/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: SPSC Queue
 */

#pragma once

#include <atomic>
#include <cstdlib>
#include <memory>
#include <algorithm>

#include "Utils.hpp"

namespace Core
{
    template<typename Type>
    class SPSCQueue;
}

/**
 * @brief The SPSC queue is a lock-free queue that only supports a Single Producer and a Single Consumer
 * The queue is really fast compared to other more flexible implementations because the fact that only two thread can simultaneously read / write
 * means that less synchronization is needed for each operation.
 * The queue supports ranged push / pop to insert multiple elements without performance impact
 *
 * @tparam Type to be inserted
 */
template<typename Type>
class alignas_double_cacheline Core::SPSCQueue
{
public:
    /** @brief Buffer structure containing all cells */
    struct Buffer
    {
        Type *data { nullptr };
        std::size_t capacity { 0u };
    };

    /** @brief Local thread cache */
    struct Cache
    {
        Buffer buffer {};
        std::size_t value { 0u };
    };

    /** @brief Default constructor, the queue is unsafe to use until 'resize' is called */
    SPSCQueue(void) noexcept = default;

    /** @brief Default constructor initialize the queue
     * If 'usedAsBuffer' is true, capacity will be increased by 1 because the queue implementation needs one unused value when the queue is full */
    SPSCQueue(const std::size_t capacity, const bool usedAsBuffer = true) noexcept;

    /** @brief Destruct and release all memory (unsafe) */
    ~SPSCQueue(void) noexcept_destructible(Type);

    /** @brief Push a single element into the queue
     *  @return true if the element has been inserted */
    template<typename ...Args>
    [[nodiscard]] std::enable_if_t<std::is_constructible_v<Type, Args...>, bool>
            push(Args &&...args)
        noexcept_constructible(Type, Args...);

    /** @brief Pop a single element from the queue
     *  @return true if an element has been extracted */
    [[nodiscard]] bool pop(Type &value)
        noexcept(nothrow_destructible(Type) && nothrow_forward_assignable(Type));

    /** @brief Push exactly 'count' elements into the queue
     *  @tparam ForceCopy If true, will prevent to move construct elements
     *  @return Success on true */
    template<typename InputIterator>
    [[nodiscard]] bool tryPushRange(const InputIterator from, const InputIterator to)
        noexcept_forward_iterator_constructible(InputIterator)
        { return pushRangeImpl<false>(from, to); }

    /** @brief Pop exactly 'count' elements from the queue
     *  @return Success on true */
    template<typename OutputIterator>
    [[nodiscard]] bool tryPopRange(const OutputIterator from, const OutputIterator to)
        noexcept(nothrow_destructible(Type) && nothrow_forward_assignable(Type))
        { return popRangeImpl<false>(from, to); }

    /** @brief Push up to 'count' elements into the queue
     *  @tparam ForceCopy If true, will prevent to move construct elements
     *  @return The number of extracted elements */
    template<typename InputIterator>
    [[nodiscard]] std::size_t pushRange(const InputIterator from, const InputIterator to)
        noexcept_forward_assignable(Type)
        { return pushRangeImpl<true>(from, to); }

    /** @brief Pop up to 'count' elements from the queue
     *  @return The number of extracted elements */
    template<typename OutputIterator>
    [[nodiscard]] std::size_t popRange(const OutputIterator from, const OutputIterator to)
        noexcept(nothrow_destructible(Type) && nothrow_forward_assignable(Type))
        { return popRangeImpl<true>(from, to); }

    /** @brief Clear all elements of the queue (unsafe) */
    void clear(void) noexcept_destructible(Type);

    /** @brief Get the size of the queue */
    [[nodiscard]] std::size_t size(void) const noexcept;

    /** @brief Resize the SPSCQueue */
    void resize(const std::size_t capacity, const bool usedAsBuffer = true) noexcept;

private:
    alignas_cacheline std::atomic<size_t> _tail { 0u }; // Tail accessed by both producer and consumer
    alignas_cacheline Cache _tailCache {}; // Cache accessed by consumer thread

    alignas_cacheline std::atomic<size_t> _head { 0u }; // Head accessed by both producer and consumer
    alignas_cacheline Cache _headCache {}; // Cache accessed by producer thread

    /** @brief Copy and move constructors disabled */
    SPSCQueue(const SPSCQueue &other) = delete;
    SPSCQueue(SPSCQueue &&other) = delete;

    template<bool AllowLess, typename InputIterator>
    [[nodiscard]] std::size_t pushRangeImpl(const InputIterator from, const InputIterator to)
        noexcept_forward_iterator_constructible(InputIterator);

    template<bool AllowLess, typename OutputIterator>
    [[nodiscard]] std::size_t popRangeImpl(const OutputIterator from, const OutputIterator to)
        noexcept(nothrow_destructible(Type) && nothrow_forward_assignable(Type));
};

static_assert_sizeof(Core::SPSCQueue<int>, 4 * Core::CacheLineSize);
static_assert_alignof_double_cacheline(Core::SPSCQueue<int>);

#include "SPSCQueue.ipp"