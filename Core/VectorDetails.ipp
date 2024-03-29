/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: VectorDetails
 */

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
template<typename ...Args>
inline std::enable_if_t<std::is_constructible_v<Type, Args...>, Type &> Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::push(Args &&...args)
    noexcept(nothrow_constructible(Type, Args...) && nothrow_forward_constructible(Type) && nothrow_destructible(Type))
{
    if (!data())
        reserveUnsafe<false>(2);
    else if (sizeUnsafe() == capacityUnsafe())
        grow();
    const Range currentSize = sizeUnsafe();
    Type * const elem = dataUnsafe() + currentSize;
    setSize(static_cast<Range>(currentSize + static_cast<Range>(1)));
    new (elem) Type(std::forward<Args>(args)...);
    return *elem;
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
inline void Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::pop(void) noexcept_destructible(Type)
{
    const Range desiredSize = sizeUnsafe() - static_cast<Range>(1);

    dataUnsafe()[desiredSize].~Type();
    setSize(desiredSize);
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
inline typename Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::Iterator
    Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::insertDefault(Iterator pos, const Range count)
    noexcept(nothrow_default_constructible(Type) && nothrow_forward_constructible(Type) && nothrow_destructible(Type))
{
    if (!count)
        return end();
    else if (pos == nullptr)  {
        resize(count);
        return beginUnsafe();
    }
    const auto currentData = dataUnsafe();
    const Range currentSize = sizeUnsafe();
    auto currentBegin = beginUnsafe();
    auto currentEnd = endUnsafe();
    Range position = pos - currentBegin;
    if (const Range currentCapacity = capacityUnsafe(), total = currentSize + count; total > currentCapacity)  {
        const Range desiredCapacity = currentCapacity + static_cast<Range>(std::max(currentCapacity, count));
        const auto tmpData = allocate(desiredCapacity);
        setData(tmpData);
        setSize(total);
        setCapacity(desiredCapacity);
        if constexpr (IsSmallOptimized) {
            if (tmpData == currentData) {
                if (const Range after = currentSize - position; after > count) {
                    std::uninitialized_move(currentEnd - count, currentEnd, currentEnd);
                    std::move_backward(currentBegin + position, currentEnd - count, currentEnd);
                } else
                    std::uninitialized_move(currentBegin + position, currentEnd, currentEnd + count - after);
                std::uninitialized_default_construct_n(tmpData + position, count);
                return tmpData + position;
            }
        }
        std::uninitialized_move_n(currentBegin, position, tmpData);
        std::uninitialized_move(currentBegin + position, currentEnd, tmpData + position + count);
        std::uninitialized_default_construct_n(tmpData + position, count);
        deallocate(currentData, currentCapacity);
        return tmpData + position;
    } else if (const Range after = currentSize - position; after > count) {
        std::uninitialized_move(currentEnd - count, currentEnd, currentEnd);
        std::move_backward(currentBegin + position, currentEnd - count, currentEnd);
    } else
        std::uninitialized_move(currentBegin + position, currentEnd, currentEnd + count - after);
    std::uninitialized_default_construct_n(currentBegin + position, count);
    setSize(currentSize + count);
    return currentBegin + position;
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
inline typename Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::Iterator
    Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::insertCopy(
        Iterator pos, const Range count, const Type &value)
    noexcept(nothrow_copy_constructible(Type) && nothrow_forward_constructible(Type) && nothrow_destructible(Type))
{
    if (!count)
        return end();
    else if (pos == nullptr)  {
        resize(count, value);
        return beginUnsafe();
    }
    const auto currentData = dataUnsafe();
    const Range currentSize = sizeUnsafe();
    auto currentBegin = beginUnsafe();
    auto currentEnd = endUnsafe();
    Range position = pos - currentBegin;
    if (const Range currentCapacity = capacityUnsafe(), total = currentSize + count; total > currentCapacity)  {
        const Range desiredCapacity = currentCapacity + static_cast<Range>(std::max(currentCapacity, count));
        const auto tmpData = allocate(desiredCapacity);
        setData(tmpData);
        setSize(total);
        setCapacity(desiredCapacity);
        if constexpr (IsSmallOptimized) {
            if (tmpData == currentData) {
                if (const Range after = currentSize - position; after > count) {
                    std::uninitialized_move(currentEnd - count, currentEnd, currentEnd);
                    std::move_backward(currentBegin + position, currentEnd - count, currentEnd);
                } else
                    std::uninitialized_move(currentBegin + position, currentEnd, currentEnd + count - after);
                std::fill_n(tmpData + position, count, value);
                return tmpData + position;
            }
        }
        std::uninitialized_move_n(currentBegin, position, tmpData);
        std::uninitialized_move(currentBegin + position, currentEnd, tmpData + position + count);
        std::fill_n(tmpData + position, count, value);
        deallocate(currentData, currentCapacity);
        return tmpData + position;
    } else if (const Range after = currentSize - position; after > count) {
        std::uninitialized_move(currentEnd - count, currentEnd, currentEnd);
        std::move_backward(currentBegin + position, currentEnd - count, currentEnd);
    } else
        std::uninitialized_move(currentBegin + position, currentEnd, currentEnd + count - after);
    std::fill_n(currentBegin + position, count, value);
    setSize(currentSize + count);
    return currentBegin + position;
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
template<typename InputIterator>
inline typename Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::Iterator
    Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::insert(
        Iterator pos, InputIterator from, InputIterator to)
    noexcept(nothrow_forward_iterator_constructible(InputIterator) && nothrow_forward_constructible(Type) && nothrow_destructible(Type))
{
    const auto count = static_cast<Range>(std::distance(from, to));
    Range position;

    if (!count)
        return end();
    else if (pos == Iterator())  {
        reserve(count);
        position = 0;
    } else
        position = static_cast<Range>(std::distance(beginUnsafe(), pos));
    const auto currentData = dataUnsafe();
    const Range currentSize = sizeUnsafe();
    auto currentBegin = beginUnsafe();
    auto currentEnd = endUnsafe();
    if (const Range currentCapacity = capacityUnsafe(), total = currentSize + count; total > currentCapacity)  {
        const Range desiredCapacity = currentCapacity + static_cast<Range>(std::max(currentCapacity, count));
        const auto tmpData = allocate(desiredCapacity);
        setData(tmpData);
        setSize(total);
        setCapacity(desiredCapacity);
        if constexpr (IsSmallOptimized) {
            if (tmpData == currentData) {
                if (const Range after = currentSize - position; after > count) {
                    std::uninitialized_move(currentEnd - count, currentEnd, currentEnd);
                    std::move_backward(currentBegin + position, currentEnd - count, currentEnd);
                } else
                    std::uninitialized_move(currentBegin + position, currentEnd, currentEnd + count - after);
                std::copy(from, to, tmpData + position);
                return tmpData + position;
            }
        }
        std::uninitialized_move_n(currentBegin, position, tmpData);
        std::uninitialized_move(currentBegin + position, currentEnd, tmpData + position + count);
        std::copy(from, to, tmpData + position);
        deallocate(currentData, currentCapacity);
        return tmpData + position;
    } else if (const Range after = currentSize - position; after > count) {
        std::uninitialized_move(currentEnd - count, currentEnd, currentEnd);
        std::move_backward(currentBegin + position, currentEnd - count, currentEnd);
    } else
        std::uninitialized_move(currentBegin + position, currentEnd, currentEnd + count - after);
    std::copy(from, to, currentBegin + position);
    setSize(currentSize + count);
    return currentBegin + position;
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
template<typename InputIterator, typename Map>
inline typename Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::Iterator
    Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::insert(
        Iterator pos, InputIterator from, InputIterator to, Map &&map)
{
    constexpr auto MapCopy = [](InputIterator from, InputIterator to, auto &&map, Iterator begin) {
        while (from != to) {
            if constexpr (Utils::IsMoveIterator<InputIterator>::Value)
                new (begin) Type(map(std::move(*from)));
            else
                new (begin) Type(map(*from));
            ++from;
            ++begin;
        }
    };

    const auto count = static_cast<Range>(std::distance(from, to));
    Range position;

    if (!count)
        return end();
    else if (pos == Iterator())  {
        reserve(count);
        position = static_cast<Range>(0);
    } else
        position = static_cast<Range>(std::distance(beginUnsafe(), pos));
    const auto currentData = dataUnsafe();
    const Range currentSize = sizeUnsafe();
    auto currentBegin = beginUnsafe();
    auto currentEnd = endUnsafe();
    if (const Range currentCapacity = capacityUnsafe(), total = currentSize + count; total > currentCapacity)  {
        const Range desiredCapacity = currentCapacity + static_cast<Range>(std::max(currentCapacity, count));
        const auto tmpData = allocate(desiredCapacity);
        setData(tmpData);
        setSize(total);
        setCapacity(desiredCapacity);
        if constexpr (IsSmallOptimized) {
            if (tmpData == currentData) {
                if (const Range after = currentSize - position; after > count) {
                    std::uninitialized_move(currentEnd - count, currentEnd, currentEnd);
                    std::move_backward(currentBegin + position, currentEnd - count, currentEnd);
                } else
                    std::uninitialized_move(currentBegin + position, currentEnd, currentEnd + count - after);
                MapCopy(from, to, std::forward<Map>(map), tmpData + position);
                return tmpData + position;
            }
        }
        std::uninitialized_move_n(currentBegin, position, tmpData);
        std::uninitialized_move(currentBegin + position, currentEnd, tmpData + position + count);
        MapCopy(from, to, std::forward<Map>(map), tmpData + position);
        deallocate(currentData, currentCapacity);
        return tmpData + position;
    } else if (const Range after = currentSize - position; after > count) {
        std::uninitialized_move(currentEnd - count, currentEnd, currentEnd);
        std::move_backward(currentBegin + position, currentEnd - count, currentEnd);
    } else
        std::uninitialized_move(currentBegin + position, currentEnd, currentEnd + count - after);
    MapCopy(from, to, std::forward<Map>(map), currentBegin + position);
    setSize(currentSize + count);
    return currentBegin + position;
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
inline void Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::erase(Iterator from, Iterator to)
    noexcept(nothrow_forward_constructible(Type) && nothrow_destructible(Type))
{
    if (from == to)
        return;
    const auto end = endUnsafe();
    setSize(sizeUnsafe() - static_cast<Range>(std::distance(from, to)));
    std::destroy(from, to);
    std::uninitialized_move(to, end, from);
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
inline void Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::resize(const Range count)
    noexcept(nothrow_default_constructible(Type) && nothrow_destructible(Type))
{
    if (!count)  {
        clear();
        return;
    } else if (!data())
        reserveUnsafe<false>(count);
    else if (capacityUnsafe() < count)
        reserveUnsafe<true>(count);
    else
        clearUnsafe();
    setSize(count);
    std::uninitialized_value_construct_n(dataUnsafe(), count);
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
inline void Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::resize(const Range count, const Type &value)
    noexcept(nothrow_copy_constructible(Type) && nothrow_forward_constructible(Type) && nothrow_destructible(Type))
{
    if (!count)  {
        clear();
        return;
    } else if (!data())
        reserveUnsafe<false>(count);
    else if (capacityUnsafe() < count)
        reserveUnsafe<true>(count);
    else
        clearUnsafe();
    setSize(count);
    std::uninitialized_fill_n(dataUnsafe(), count, value);
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
template<typename InputIterator>
inline std::enable_if_t<std::is_constructible_v<Type, decltype(*std::declval<InputIterator>())>, void>
        Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::resize(InputIterator from, InputIterator to)
    noexcept(nothrow_forward_iterator_constructible(InputIterator) && nothrow_forward_constructible(Type) && nothrow_destructible(Type))
{
    const Range count = static_cast<Range>(std::distance(from, to));

    if (!count)  {
        clear();
        return;
    } else if (!data())
        reserveUnsafe<false>(count);
    else if (capacityUnsafe() < count)
        reserveUnsafe<true>(count);
    else
        clearUnsafe();
    setSize(count);
    std::uninitialized_copy(from, to, beginUnsafe());
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
template<typename InputIterator, typename Map>
inline void Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::resize(InputIterator from, InputIterator to, Map &&map)
{
    const auto count = static_cast<Range>(std::distance(from, to));

    if (!count)  {
        clear();
        return;
    } else if (!data())
        reserveUnsafe<false>(count);
    else if (capacityUnsafe() < count)
        reserveUnsafe<true>(count);
    else
        clearUnsafe();
    setSize(count);
    auto begin = beginUnsafe();
    while (from != to) {
        if constexpr (Utils::IsMoveIterator<InputIterator>::Value)
            new (begin) Type(map(std::move(*from)));
        else
            new (begin) Type(map(*from));
        ++from;
        ++begin;
    }
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
inline void Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::clear(void) noexcept_destructible(Type)
{
    if (data())
        clearUnsafe();
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
inline void Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::clearUnsafe(void) noexcept_destructible(Type)
{
    std::destroy_n(dataUnsafe(), sizeUnsafe());
    setSize(0);
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
inline void Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::release(void) noexcept_destructible(Type)
{
    if (data())
        releaseUnsafe();
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
inline void Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::releaseUnsafe(void) noexcept_destructible(Type)
{
    const auto currentData = dataUnsafe();
    const Range currentCapacity = capacityUnsafe();

    clearUnsafe();
    setCapacity(static_cast<Range>(0));
    setData(nullptr);
    deallocate(currentData, currentCapacity);
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
inline bool Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::reserve(const Range capacity)
    noexcept(nothrow_forward_constructible(Type) && nothrow_destructible(Type))
{
    if (data())
        return reserveUnsafe<true>(capacity);
    else
        return reserveUnsafe<false>(capacity);
}


template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
template<bool IsSafe>
inline bool Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::reserveUnsafe(const Range capacity)
    noexcept(nothrow_forward_constructible(Type) && nothrow_destructible(Type))
{
    if constexpr (IsSafe) {
        const Range currentCapacity = capacityUnsafe();
        if (currentCapacity >= capacity)
            return false;
        const Range currentSize = sizeUnsafe();
        const auto currentData = dataUnsafe();
        const auto tmpData = allocate(capacity);
        setData(tmpData);
        setSize(currentSize);
        setCapacity(capacity);
        if constexpr (IsSmallOptimized) {
            if (tmpData == currentData)
                return false;
        }
        std::uninitialized_move_n(currentData, currentSize, tmpData);
        std::destroy_n(currentData, currentSize);
        deallocate(currentData, currentCapacity);
        return true;
    } else {
        if (capacity == 0)
            return false;
        setData(allocate(capacity));
        setSize(0);
        setCapacity(capacity);
        return true;
    }
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
inline void Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::grow(const Range minimum)
    noexcept(nothrow_forward_constructible(Type) && nothrow_destructible(Type))
{
    const auto currentData = dataUnsafe();
    const Range currentSize = sizeUnsafe();
    const Range currentCapacity = capacityUnsafe();
    const Range desiredCapacity = static_cast<Range>(currentCapacity + static_cast<Range>(std::max(currentCapacity, minimum)));
    const auto tmpData = allocate(desiredCapacity);

    setData(tmpData);
    setSize(currentSize);
    setCapacity(desiredCapacity);
    if constexpr (IsSmallOptimized) {
        if (tmpData == currentData)
            return;
    }
    std::uninitialized_move_n(currentData, currentSize, tmpData);
    std::destroy_n(currentData, currentSize);
    deallocate(currentData, currentCapacity);
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
inline void Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::move(Range from, Range to, Range output) noexcept_ndebug
{
    coreAssert(output < from || output > to,
        throw std::logic_error("VectorDetails::move: Invalid move range"));
    ++to;
    if (output < from) {
        const auto tmp = from;
        from = output;
        output = to;
        to = tmp;
    } else if (output)
        ++output;
    const auto it = beginUnsafe();
    std::rotate(it + from, it + to, it + output);
}

template<typename Base, typename Type, typename Range, bool IsSmallOptimized>
inline bool Core::Internal::VectorDetails<Base, Type, Range, IsSmallOptimized>::operator==(const VectorDetails &other) const noexcept
{
    const Range count = size();
    const Range otherCount = other.size();

    if (count == otherCount)  {
        if (count)
            return std::equal(beginUnsafe(), endUnsafe(), other.beginUnsafe());
        else
            return true;
    } else
        return false;
}
