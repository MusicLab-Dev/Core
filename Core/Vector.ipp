/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Vector
 */

template<typename Type, typename Range>
inline void Core::Internal::VectorBase<Type, Range>::swap(VectorBase &other) noexcept
{
    std::swap(_data, other._data);
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
}
