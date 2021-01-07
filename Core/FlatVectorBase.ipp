/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: FlatVector
 */

template<typename Type, typename Range>
inline const Type *Core::Internal::FlatVectorBase<Type, Range>::data(void) const noexcept
{
    if (_ptr) [[likely]]
        return dataUnsafe();
    else
        return nullptr;
}

template<typename Type, typename Range>
inline void Core::Internal::FlatVectorBase<Type, Range>::steal(FlatVectorBase &other) noexcept
{
    std::swap(_ptr, other._ptr);
}