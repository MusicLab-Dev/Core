/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: String
 */

#pragma once

#include "StringDetails.hpp"
#include "Vector.hpp"

namespace Core
{
    /**
     * @brief String that has its size and capacity close to the data pointer
     *  The string is non-null terminated
     *
     * @tparam Type Type of character
     * @tparam Range Range of container
     */
    template<typename Type, typename Range = std::size_t>
    using StringBase = Internal::StringDetails<Vector<Type, Range>, Type, Range>;

    /** @brief 16 bytes string with a reduced range
     *  The string is non-null terminated */
    template<typename Type>
    using TinyStringBase = StringBase<Type, std::uint32_t>;

    /** @brief 24 bytes string using signed char
     *  The string is non-null terminated */
    using String = StringBase<char, std::size_t>;

    /** @brief 16 bytes string using signed char with a reduced range
     *  The string is non-null terminated */
    using TinyString = TinyStringBase<char>;
}

static_assert_sizeof(Core::String, 3 * Core::CacheLineEighthSize);
static_assert_sizeof_quarter_cacheline(Core::TinyString);