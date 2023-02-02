// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#pragma warning(push)
#pragma warning(disable: 4619) // #pragma warning : there is no warning number '...'
#pragma warning(disable: 5264) // '...': '...' variable is not used
#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <tuple>
#include <sstream>
#pragma warning(pop)
#include <map>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <memory>
#include <vector>
#include <typeinfo>
#include <type_traits>
#include <cstdlib>
#include <iterator>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <ciso646>

#pragma warning(disable: 5045) // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
#pragma warning(disable: 4820) // '...': '...' bytes padding added after data member '...' 
#pragma warning(disable: 4626) // '...': assignment operator was implicitly defined as deleted
#pragma warning(disable: 4514) // '...': unreferenced inline function has been removed
#pragma warning(disable: 5039) // '...': pointer or reference to potentially throwing function passed to 'extern "C"' function under -EHc. Undefined behavior may occur

#pragma warning(push)
#pragma warning(disable: 26814) // The const variable '...' can be computed at compile-time. Consider using constexpr (con.5).

#include "Matrix.h"

#pragma warning(pop)


#pragma warning(disable: 4100) // '...': unreferenced formal parameter
#pragma warning(disable: 26440) // Function '...' can be declared '...' (f.6).
#pragma warning(disable: 26455) // Default constructor should not throw. Declare it '...' (f.6).
#pragma warning(disable: 26432) // If you define or delete any default operation in the type 'class TestSuite', define or delete them all (c.21).
#pragma warning(disable: 26472) // Don't use a static_cast for arithmetic conversions. Use brace initialization, gsl::narrow_cast or gsl::narrow (type.1).
#pragma warning(disable: 26497) // You can attempt to make '...' constexpr unless it contains any undefined behavior (f.4).
#pragma warning(disable: 26429) // Symbol '...' is never tested for nullness, it can be marked as not_null (f.23).

#endif //PCH_H
