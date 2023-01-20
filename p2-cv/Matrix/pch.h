// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#pragma warning(push)
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

#include "Matrix.h"
#include "Matrix_test_helpers.h"


#pragma warning(disable: 4100) // '...': unreferenced formal parameter

#endif //PCH_H
