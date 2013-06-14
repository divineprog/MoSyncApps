#include "TestRunner.h"

// Test defined without macros.
void MyFirstTest(Test* __test)
{
	__test->shouldHold(1==1, "1==1");
}
Test __MyFirstTest(MyFirstTest, "MyFirstTest");

// Test defined with macros.
TestCaseBegin(MySecondTest)
{
	ShouldHold(1==1);
}
TestCaseEnd(MySecondTest)

// Test defined with macros.
TestCaseBegin(MyFailingTest)
{
	ShouldHold(1==2);
}
TestCaseEnd(MyFailingTest)
