/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Sorted Vector
 */

#pragma once

#include "SortedVectorDetails.hpp"
#include "VectorBase.hpp"

namespace Core
{
    template<typename Type, typename Range = std::size_t, typename Compare = std::less<Type>>
    using SortedVector = Internal::SortedVectorDetails<Internal::VectorBase<Type, Range>, Type, Range, Compare>;

    template<typename Type, typename Compare = std::less<Type>>
    using TinySortedVector = SortedVector<Type, std::uint32_t, Compare>;
}