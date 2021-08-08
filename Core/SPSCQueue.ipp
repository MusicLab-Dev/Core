/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: SPSC Queue
 */

template<typename Type>
Core::SPSCQueue<Type>::SPSCQueue(const std::size_t capacity, const bool usedAsBuffer) noexcept
{
    _tailCache.buffer.capacity = capacity + usedAsBuffer;
    _tailCache.buffer.data = reinterpret_cast<Type *>(Utils::AlignedAlloc<alignof(Type)>(sizeof(Type) * _tailCache.buffer.capacity));
    _headCache.buffer = _tailCache.buffer;
}

template<typename Type>
Core::SPSCQueue<Type>::~SPSCQueue(void) noexcept_destructible(Type)
{
    clear();
    Utils::AlignedFree(_tailCache.buffer.data);
}

template<typename Type>
template<typename ...Args>
inline std::enable_if_t<std::is_constructible_v<Type, Args...>, bool>
        Core::SPSCQueue<Type>::push(Args &&...args)
    noexcept_constructible(Type, Args...)
{
    const auto tail = _tail.load(std::memory_order_relaxed);
    auto next = tail + 1;

    if (next == _tailCache.buffer.capacity)
        next = 0;
    if (auto head = _tailCache.value; next == head) {
        head = _tailCache.value = _head.load(std::memory_order_acquire);
        if (next == head)
            return false;
    }
    new (_tailCache.buffer.data + tail) Type { std::forward<Args>(args)... };
    _tail.store(next, std::memory_order_release);
    return true;
}

template<typename Type>
inline bool Core::SPSCQueue<Type>::pop(Type &value)
    noexcept(nothrow_destructible(Type) && nothrow_forward_assignable(Type))
{
    const auto head = _head.load(std::memory_order_relaxed);

    if (auto tail = _headCache.value; head == tail) {
        tail = _headCache.value = _tail.load(std::memory_order_acquire);
        if (head == tail)
            return false;
    }
    auto *elem = reinterpret_cast<Type *>(_headCache.buffer.data + head);
    auto next = head + 1;
    if (next == _headCache.buffer.capacity)
        next = 0;
    if constexpr (std::is_move_assignable_v<Type>)
        value = std::move(*elem);
    else
        value = *elem;
    elem->~Type();
    _head.store(next, std::memory_order_release);
    return true;
}

template<typename Type>
template<bool AllowLess, typename InputIterator>
inline std::size_t Core::SPSCQueue<Type>::pushRangeImpl(const InputIterator from, const InputIterator to)
    noexcept_forward_iterator_constructible(InputIterator)
{
    std::size_t toPush = to - from;
    const auto tail = _tail.load(std::memory_order_relaxed);
    const auto capacity = _tailCache.buffer.capacity;
    auto head = _tailCache.value;
    auto available = capacity - (tail - head);

    if (available > capacity)
        available -= capacity;
    if (toPush >= available) {
        head = _tailCache.value = _head.load(std::memory_order_acquire);
        available = capacity - (tail - head);
        if (available > capacity)
            available -= capacity;
        if (toPush >= available) {
            if constexpr (AllowLess)
                toPush = available - 1;
            else
                return 0;
        }
    }
    auto next = tail + toPush;
    if (next >= capacity) {
        next -= capacity;
        const auto split = toPush - next;
        std::uninitialized_move_n(from, split, _tailCache.buffer.data + tail);
        std::destroy_n(from, split);
        std::uninitialized_move_n(from + split, next, _tailCache.buffer.data);
        std::destroy_n(from + split, next);
    } else {
        std::uninitialized_move_n(from, toPush, _tailCache.buffer.data + tail);
        std::destroy_n(from, toPush);
    }
    _tail.store(next, std::memory_order_release);
    return toPush;
}

template<typename Type>
template<bool AllowLess, typename OutputIterator>
inline std::size_t Core::SPSCQueue<Type>::popRangeImpl(const OutputIterator from, const OutputIterator to)
    noexcept(nothrow_destructible(Type) && nothrow_forward_assignable(Type))
{
    std::size_t toPop = to - from;
    const auto head = _head.load(std::memory_order_relaxed);
    const auto capacity = _headCache.buffer.capacity;
    auto tail = _headCache.value;
    auto available = tail - head;

    if (available > capacity)
        available += capacity;
    if (toPop >= available) {
        tail = _headCache.value = _tail.load(std::memory_order_acquire);
        available = tail - head;
        if (available > capacity)
            available += capacity;
        if (toPop > available) {
            if constexpr (AllowLess)
                toPop = available;
            else
                return 0;
        }
    }
    auto next = head + toPop;
    if (next >= capacity) {
        next -= capacity;
        const auto split = toPop - next;
        std::copy_n(std::make_move_iterator(_headCache.buffer.data + head), split, from);
        std::destroy_n(_headCache.buffer.data + head, split);
        std::copy_n(std::make_move_iterator(_headCache.buffer.data), next, from + split);
        std::destroy_n(_headCache.buffer.data, next);
    } else {
        std::copy_n(std::make_move_iterator(_headCache.buffer.data + head), toPop, from);
        std::destroy_n(_headCache.buffer.data + head, toPop);
    }
    _head.store(next, std::memory_order_release);
    return toPop;
}

template<typename Type>
inline void Core::SPSCQueue<Type>::clear(void) noexcept_destructible(Type)
{
    for (Type type; pop(type););
}

template<typename Type>
inline std::size_t Core::SPSCQueue<Type>::size(void) const noexcept
{
    const auto tail = _tail.load(std::memory_order_seq_cst);
    const auto capacity = _tailCache.buffer.capacity;
    const auto head = _head.load(std::memory_order_seq_cst);
    auto available = tail - head;
    if (available > capacity)
        available += capacity;
    return available;
}

template<typename Type>
inline void Core::SPSCQueue<Type>::resize(const std::size_t capacity, const bool usedAsBuffer) noexcept
{
    const auto totalCapacity = capacity + usedAsBuffer;
    auto data = _tailCache.buffer.data;

    if (_tailCache.buffer.capacity != totalCapacity)
        data = Utils::AlignedAlloc<alignof(Type), Type>(sizeof(Type) * totalCapacity);

    if (_tailCache.buffer.data && _tailCache.buffer.data != data)
        Utils::AlignedFree(_tailCache.buffer.data);

    _tailCache.buffer.capacity = totalCapacity;
    _tailCache.buffer.data = data;
    _tailCache.value = 0u;
    _tail.store(0u);

    _headCache.buffer = _tailCache.buffer;
    _headCache.value = 0u;
    _head.store(0u);
}