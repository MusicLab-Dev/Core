/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: HeapArray
 */

template<typename Type>
template<typename ...Args>
std::enable_if_t<std::is_constructible_v<Type, Args...>, void>
        Core::HeapArray<Type>::allocate(const std::size_t size, Args &&...args)
    noexcept(nothrow_ndebug && nothrow_constructible(Type, Args...) && nothrow_destructible(Type))
{
    if constexpr (!std::is_trivially_destructible_v<Type>) {
        for (auto &elem : *this)
            elem.~Type();
    }
    if (size && _size != size) {
        if (_size)
            Utils::AlignedFree(_data);
        _data = reinterpret_cast<Type *>(Utils::AlignedAlloc<alignof(Type)>(sizeof(Type) * size));
        coreAssert(_data,
            throw std::runtime_error("Core::HeapArray::allocate: Malloc failed"));
    }
    _size = size;
    for (auto i = 0ul; i < _size; ++i)
        new (&_data[i]) Type(args...);
}

template<typename Type>
inline void Core::HeapArray<Type>::release(void) noexcept_destructible(Type)
{
    if (!_data)
        return;
    if constexpr (!std::is_trivially_destructible_v<Type>) {
        for (auto &elem : *this)
            elem.~Type();
    }
    Utils::AlignedFree(_data);
    _data = nullptr;
    _size = 0;
}
