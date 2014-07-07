#include "UnitTest.h"
#include "TestSuite.h"
using namespace core;

UnitTest::UnitTest(const char* name, const char* file, const long line)
	: Name(name), File(file), Line(line)
{
}

UnitTest::~UnitTest()
{
}
