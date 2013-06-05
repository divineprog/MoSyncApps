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
