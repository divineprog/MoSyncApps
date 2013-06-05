#include "TestRunner.h"

// Test defined without macros.
void MyFirstTest()
{
	TestRunner::mInstance->registerTestResult(
	    1==1, "MyFirstTest", "1==1");
}

// Test defined with macros.
void MySecondTest()
{
	TestCase("MySecondTest");
	ShouldHold(1==1);
}

// Test defined with macros.
void MyFailingTest()
{
	TestCase("MySecondTest");
	ShouldHold(1==2);
}

void RunAllTests()
{
	MyFirstTest();
	MySecondTest();
	MyFailingTest();
}
