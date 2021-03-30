/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: FlatString
 */

#pragma once

#include "StringDetails.hpp"
#include "FlatVector.hpp"

namespace Core
{
    /**
     * @brief 8 bytes vector that allocates its size and capacity on the heap
     * The string is non-null terminated
     *
     * @tparam Type Type of character
     * @tparam Range Range of container
     */
    template<typename Type, typename Range = std::size_t>
    using FlatStringBase = Internal::StringDetails<FlatVector<Type, Range, Internal::NoCustomHeaderType>, Type, Range>;

    /** @brief 8 bytes string using signed char and size_t range
     *  The string is non-null terminated */
    using FlatString = FlatStringBase<char, std::size_t>;

    /** @brief 8 bytes string using signed char with a reduced range
     *  The string is non-null terminated */
    using TinyFlatString = FlatStringBase<char, std::uint32_t>;
}

static_assert_sizeof(Core::FlatString, Core::CacheLineEighthSize);
static_assert_sizeof(Core::TinyFlatString, Core::CacheLineEighthSize);