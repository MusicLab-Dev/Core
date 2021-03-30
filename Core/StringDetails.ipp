/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: String details
 */

template<typename Base, typename Type, typename Range>
Core::Internal::StringDetails<Base, Type, Range> Core::Internal::StringDetails<Base, Type, Range>::operator+(const StringDetails &other) noexcept
{
    StringDetails res;
    res.reserve(size() + other.size());
    res.insert(res.end(), begin(), end());
    res.insert(res.end(), other.begin(), other.end());
    return res;
}

template<typename Base, typename Type, typename Range>
Core::Internal::StringDetails<Base, Type, Range> Core::Internal::StringDetails<Base, Type, Range>::operator+(const char * const cstring) noexcept
{
    StringDetails res;
    const auto length = SafeStrlen(cstring);
    res.reserve(size() + length);
    res.insert(res.end(), begin(), end());
    res.insert(res.end(), cstring, cstring + length);
    return res;
}

template<typename Base, typename Type, typename Range>
Core::Internal::StringDetails<Base, Type, Range> Core::Internal::StringDetails<Base, Type, Range>::operator+(const std::basic_string<Type> &other) noexcept
{
    StringDetails res;
    res.reserve(size() + other.size());
    res.insert(res.end(), begin(), end());
    res.insert(res.end(), other.begin(), other.end());
    return res;
}

template<typename Base, typename Type, typename Range>
inline Core::Internal::StringDetails<Base, Type, Range> Core::Internal::StringDetails<Base, Type, Range>::operator+(const std::basic_string_view<Type> &other) noexcept
{
    StringDetails res;
    res.reserve(size() + other.size());
    res.insert(res.end(), begin(), end());
    res.insert(res.end(), other.begin(), other.end());
    return res;
}

template<typename Base, typename Type, typename Range>
inline const char *Core::Internal::StringDetails<Base, Type, Range>::c_str(void) const noexcept
{
    if (!size())
        return nullptr;
    else if (size() == capacity())
        const_cast<StringDetails *>(this)->grow(1);
    *const_cast<StringDetails *>(this)->end() = '\0';
    return dataUnsafe();
}

template<typename Base, typename Type, typename Range>
inline std::size_t Core::Internal::StringDetails<Base, Type, Range>::SafeStrlen(const char * const cstring) noexcept
{
    if (!cstring)
        return 0;
    else
        return std::strlen(cstring);
}