/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Sorted FlatVector
 */

#pragma once

#include "SortedVectorDetails.hpp"
#include "FlatVectorBase.hpp"

namespace Core
{
    /**
     * @brief 8 bytes vector that allocates its size and capacity on the heap
     * The vector guarantee that it will be sorted at any given time
     *
     * @tparam Type Internal type in container
     * @tparam Range Range of container
     * @tparam Compare Compare operator
     */
    template<typename Type, typename Range = std::size_t, typename Compare = std::less<Type>, typename CustomHeaderType = void>
    using SortedFlatVector = Internal::SortedVectorDetails<Internal::FlatVectorBase<Type, Range, CustomHeaderType>, Type, Range, Compare>;

    /** @brief 8 bytes vector using signed char with a reduced range
     * The vector guarantee that it will be sorted at any given time */
    template<typename Type, typename Compare = std::less<Type>, typename CustomHeaderType = void>
    using SortedTinyFlatVector = SortedFlatVector<Type, std::uint32_t, Compare, CustomHeaderType>;
}