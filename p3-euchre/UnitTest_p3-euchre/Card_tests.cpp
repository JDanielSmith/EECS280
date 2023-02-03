#include "pch.h"
#include "CppUnitTest.h"

#include "unit_test_framework.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTestp3euchre
{

TEST_CLASS(Card_tests) { public:
#include "../Card/Card_tests.cpp"
};

}