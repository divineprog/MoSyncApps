#include <conprint.h>
#include "TestRunner.h"

// ********** Class TestRunner **********

TestRunner* TestRunner::mInstance = new TestRunner();

TestRunner::TestRunner()
{
	mPassedTests = 0;
	mFailedTests = 0;
}

TestRunner::~TestRunner()
{
}

void TestRunner::addTest(Test* test)
{
	mTests.add(test);
}

void TestRunner::runAllTests()
{
	for (int i = 0; i < mTests.size(); ++i)
	{
		Test* test = mTests[i];
		test->mTestFun(test);
	}
}

void TestRunner::registerTestResult(
	bool testPassed,
	MAUtil::String testName,
	MAUtil::String testLabel)
{
	if (!testPassed)
	{
		mFailedTests++;
		printf("Test failed: %s %s\n",
			testName.c_str(),
			testLabel.c_str());
	}
	else
	{
		mPassedTests++;
	}
}

void TestRunner::reportResult()
{
	printf("Tests passed: %i\nTests failed: %i\n",
		mPassedTests,
		mFailedTests);
}

// ********** Class TestCase **********

// Class used to register tests.
Test::Test(TestFun testFun, MAUtil::String testName)
{
	mTestFun = testFun;
	mTestName = testName;
	TestRunner::mInstance->addTest(this);
}

void Test::shouldHold(bool cond, MAUtil::String label)
{
	TestRunner::mInstance->registerTestResult(cond, mTestName, label);
}
