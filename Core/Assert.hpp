/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Assert helper
 */

#pragma once

#ifdef NDEBUG // Release mode

# define CORE_DEBUG_BUILD false

// Do nothing
# define coreAssert(expression, onError) static_cast<void>(0)
# define coreAssertFallback(expression, onFallback, onError) static_cast<void>(0)
# define coreDebugThrow(exception) std::terminate();

#else // Debug mode

# define CORE_DEBUG_BUILD true

# define coreAssert(expression, onError) if (!(expression)) onError
# define coreAssertFallback(expression, onFallback, onError) if (!(expression)) { onFallback; onError; }
# define coreDebugThrow(exception) throw exception;
#endif

/** @brief Exception helper */
#define nothrow_ndebug !CORE_DEBUG_BUILD

/** @brief Helper to make a function noexcept only in release */
#define noexcept_ndebug noexcept(nothrow_ndebug)
