/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Vector
 */

template<typename Type, typename Range>
inline void Core::Internal::VectorBase<Type, Range>::steal(VectorBase &other) noexcept
{
    if (_data) {
        std::destroy(beginUnsafe(), endUnsafe());
        deallocate(_data, _capacity);
    }
    _data = other._data;
    _size = other._size;
    _capacity = other._capacity;
    other._data = nullptr;
    other._size = Range{};
    other._capacity = Range{};
}

template<typename Type, typename Range>
inline void Core::Internal::VectorBase<Type, Range>::swap(VectorBase &other) noexcept
{
    std::swap(_data, other._data);
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
}
