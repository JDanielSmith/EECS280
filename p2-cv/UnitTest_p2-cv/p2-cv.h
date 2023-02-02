#pragma once

#include "CppUnitTest.h"

#define TEST(name) TEST_METHOD(name)

#define ASSERT_EQUAL(a, b) Microsoft::VisualStudio::CppUnitTestFramework::Assert::AreEqual(a, b)
