#include "pch.h"
#include "CppUnitTest.h"

#include "unit_test_framework.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTestp3euchre
{

TEST_CLASS(Pack_tests) { public:
#include "../p3-euchre/Pack_tests.cpp"
};

}