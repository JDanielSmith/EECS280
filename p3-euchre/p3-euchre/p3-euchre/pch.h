// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#pragma warning(push)
#pragma warning(disable: 4668) // '...' is not defined as a preprocessor macro, replacing with '...' for '...'
#include <limits.h>
#pragma warning(pop)

#include "Card.h"
#include "Pack.h"
#include "Player.h"

#pragma warning(disable: 5045) // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
#pragma warning(disable: 4820) // '...': '...' bytes padding added after data member '...'
#pragma warning(disable: 4514) // '...': unreferenced inline function has been removed
#pragma warning(disable: 5039) // '...': pointer or reference to potentially throwing function passed to '...' function under -EHc. Undefined behavior may occur if this function throws an exception. 

#endif //PCH_H
