/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Kube hash function
 */

#pragma once

#include <string_view>

namespace Core
{
    /** @brief The result type of the hash function */
    using HashedName = std::uint32_t;

    /** @brief The hash offset */
    constexpr HashedName HashOffset = 4294967291u;

    /** @brief Compile-time string hashing */
    constexpr HashedName Hash(char const *str, std::size_t len) noexcept {
        HashedName h = HashOffset;
        for (std::size_t i = 0ul; i < len; ++i)
            h = 31ul * h + str[i];
        return h;
    }

    /** @brief Compile-time char hashing */
    constexpr HashedName Hash(const char c) noexcept { return 31 * 4294967291L + c; }

    /** @brief Compile-time string-view hashing */
    constexpr HashedName Hash(const std::string_view &str) { return Hash(str.data(), str.length()); }

    namespace Literal
    {
        /** @brief Compile-time string hashing literal */
        constexpr HashedName operator ""_hash(char const *str, std::size_t len) { return Hash(str, len); }

        static_assert(""_hash == HashOffset, "There is an error in compile-time hashing algorithm");
    }
}
