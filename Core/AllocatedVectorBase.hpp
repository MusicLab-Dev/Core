/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Base data of a vector
 */

#pragma once

#include "VectorBase.hpp"

namespace Core::Internal
{
    template<typename Type, typename Range, auto AllocateFunc, auto DeallocateFunc>
    class AllocatedVectorBase;
}

/** @brief Base implementation of a vector with size and capacity cached */
template<typename Type, typename Range, auto AllocateFunc, auto DeallocateFunc>
class Core::Internal::AllocatedVectorBase : public VectorBase<Type, Range>
{
protected:
    /** @brief Allocates a new buffer */
    [[nodiscard]] Type *allocate(const Range capacity) noexcept
        { return reinterpret_cast<Type *>(AllocateFunc(sizeof(Type) * capacity, alignof(Type))); }

    /** @brief Deallocates a buffer */
    void deallocate(Type * const data, const Range capacity) noexcept
        { DeallocateFunc(data, sizeof(Type) * capacity, alignof(Type)); }
};
