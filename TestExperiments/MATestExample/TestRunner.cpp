#include <conprint.h>
#include "TestRunner.h"

// ********** Class TestRunner **********

TestRunner* TestRunner::sInstance;

TestRunner* TestRunner::getInstance()
{
	if (NULL == TestRunner::sInstance)
	{
		TestRunner::sInstance = new TestRunner();
	}

	return TestRunner::sInstance;
}

TestRunner::TestRunner() :
	mSuite("TestRunner"),
	mTimeout(5000)
{
}

TestRunner::~TestRunner()
{
}

void TestRunner::addTestCase(MATest::TestCase* testCase)
{
	mSuite.addTestCase(testCase);
}

void TestRunner::addTestListener(MATest::TestListener* listener)
{
	mSuite.addTestListener(listener);
}

void TestRunner::runNextTestCase()
{
	MAUtil::Environment::getEnvironment().addTimer(this, mTimeout, 1);
	mSuite.runNextCase();
	MAUtil::Environment::getEnvironment().removeTimer(this);
}

void TestRunner::setTimeout(int ms)
{
	mTimeout = ms;
}

void TestRunner::runTimerEvent()
{
	// Currently running test timed out,
	// run next test.
	runNextTestCase();
}

// ********** Class RegisterTestCase **********

RegisterTestCase::RegisterTestCase(MATest::TestCase* testCase)
{
	TestRunner::getInstance()->addTestCase(testCase);
};

// ********** Class SimpleTestListener **********

SimpleTestListener::SimpleTestListener()
{
	mPassedTests = 0;
	mFailedTests = 0;
}

void SimpleTestListener::beginTestSuite(
	const MAUtil::String& suiteName)
{
	printf("Started running tests\n");
}

void SimpleTestListener::endTestSuite()
{
	printf("Finished running tests\n");
	printf("Tests passed: %i\n", mPassedTests);
	printf("Tests failed: %i\n", mFailedTests);
	printf("Expects failed: %i\n", (int)mExpectedAsserts.size());
	if (mExpectedAsserts.size() > 0)
	{
		printf("Expected but did not happen:\n");
		mExpectedAsserts.begin();
		for (MAUtil::Set<MAUtil::String>::Iterator iter =
				mExpectedAsserts.begin();
			iter != mExpectedAsserts.end();
			iter++)
		{
			printf("%s\n", (*iter).c_str());
		}
	}
}

void SimpleTestListener::beginTestCase(
	const MAUtil::String& testCaseName)
{
	mCurrentTestCaseName = testCaseName;
}

void SimpleTestListener::endTestCase()
{
	// Nothing done.
}

void SimpleTestListener::assertion(
	const MAUtil::String& assertionName,
	bool cond)
{
	if (!cond)
	{
		mFailedTests++;
		printf("FAIL %s: %s\n",
			mCurrentTestCaseName.c_str(),
			assertionName.c_str());
	}
	else
	{
		mExpectedAsserts.erase(
			mCurrentTestCaseName + ":" + assertionName);
		mPassedTests++;
	}
}

void SimpleTestListener::expect(
	const MAUtil::String& assertionName)
{
	printf("EXPECT %s: %s\n",
			mCurrentTestCaseName.c_str(),
			assertionName.c_str());
	mExpectedAsserts.insert(
		mCurrentTestCaseName + ":" + assertionName);
}
