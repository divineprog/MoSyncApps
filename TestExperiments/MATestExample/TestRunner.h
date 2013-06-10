#ifndef __TESTRUNNER__
#define __TESTRUNNER__

#include <MAUtil/String.h>
#include <MAUtil/Vector.h>
#include <MAUtil/Set.h>
#include <MAUtil/Environment.h>
#include <conprint.h>
#include "Test.h"

// ********** Class TestRunner **********

/**
 * Class that registers and runs test cases.
 */
class TestRunner :
	public MAUtil::TimerListener
{
public:
	// Methods.
	TestRunner();
	virtual ~TestRunner();
	void addTestCase(MATest::TestCase* testCase);
	void addTestListener(MATest::TestListener* listener);
	void runNextTestCase();
	void setTimeout(int ms);
	void runTimerEvent();
	static TestRunner* getInstance();

protected:
	// Test suite that holds test cases.
	MATest::TestSuite mSuite;

	// Timeout before test is aborted.
	int mTimeout;

	// Class variable to hold the global instance
	// of this class.
	static TestRunner* sInstance;
};

// ********** Class RegisterTestCase **********

class RegisterTestCase
{
public:
	RegisterTestCase(MATest::TestCase* testCase);
};

// ********** Class SimpleTestListener **********

class SimpleTestListener : public MATest::TestListener
{
public:
	SimpleTestListener();
	virtual void beginTestSuite(const MAUtil::String& suiteName);
	virtual void endTestSuite();
	virtual void beginTestCase(const MAUtil::String& testCaseName);
	virtual void endTestCase();
	virtual void assertion(const MAUtil::String& assertionName, bool cond);
	void expect(const MAUtil::String& assertionName);

protected:
	// Instance variables.
	int mPassedTests;
	int mFailedTests;
	MAUtil::String mCurrentTestCaseName;
	MAUtil::Set<MAUtil::String> mExpectedAsserts;
};

// ********** Macros **********

// Macros to make writing tests easier.
#define TESTCASE(testCaseClass) \
	static MATest::TestCase* sTestCase = \
		new testCaseClass(#testCaseClass); \
	static RegisterTestCase __testCaseClass(sTestCase);

#endif
