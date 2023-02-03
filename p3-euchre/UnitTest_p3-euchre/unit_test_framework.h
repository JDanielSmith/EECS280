#ifndef UNIT_TEST_FRAMEWORK_H
#define UNIT_TEST_FRAMEWORK_H
#pragma once

#include <utility>
#include <string>

#include "CppUnitTest.h"

#define TEST(name) TEST_METHOD(name)
#define TEST_MAIN() /* do nothing */

#define ASSERT_TRUE(value) Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue(value)
#define ASSERT_FALSE(value) Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsFalse(value)

template<typename T, typename U>
inline void assert_equal(const T& t, const U& u)
{
	ASSERT_TRUE(t == u);
}
inline void assert_equal(const std::string& t, const char* u)
{
	Microsoft::VisualStudio::CppUnitTestFramework::Assert::AreEqual(t.c_str(), u);
}
inline void assert_equal(int t, int u)
{
	Microsoft::VisualStudio::CppUnitTestFramework::Assert::AreEqual(t, u);
}
#define ASSERT_EQUAL(first, second) assert_equal(first, second)

#endif // UNIT_TEST_FRAMEWORK_H
