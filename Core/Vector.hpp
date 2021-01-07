/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Vector
 */

#pragma once

#include "VectorDetails.hpp"
#include "VectorBase.hpp"

namespace Core
{
    template<typename Type, typename Range = std::size_t>
    using Vector = Internal::VectorDetails<Internal::VectorBase<Type, Range>, Type, Range>;

    template<typename Type>
    using TinyVector = Vector<Type, std::uint32_t>;
}