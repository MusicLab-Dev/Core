/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: FlatVector
 */

template<typename Type, typename Range, typename CustomHeaderType>
inline const Type *Core::Internal::FlatVectorBase<Type, Range, CustomHeaderType>::data(void) const noexcept
{
    if (_ptr)
        return dataUnsafe();
    else
        return nullptr;
}

template<typename Type, typename Range, typename CustomHeaderType>
inline void Core::Internal::FlatVectorBase<Type, Range, CustomHeaderType>::steal(FlatVectorBase &other) noexcept
{
    std::swap(_ptr, other._ptr);
}