#ifndef P2_CV_unit_test_framework_h_INCLUDED_
#define P2_CV_unit_test_framework_h_INCLUDED_
#pragma once

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 26409) // Avoid calling new and delete explicitly, use std::make_unique<T> instead (r.11).
#pragma warning(disable: 26496) // The variable '..' does not change after construction, mark it as const (con.4).
#pragma warning(disable: 26461) // The pointer argument '...' for function '...' can be marked as a pointer to const (con.3).
#pragma warning(disable: 26481) // Don't use pointer arithmetic. Use span instead (bounds.1).
#endif

#include "unit_test_framework_.h"

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif  // P2_CV_unit_test_framework_h_INCLUDED_
