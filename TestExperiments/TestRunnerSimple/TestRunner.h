#ifndef __TESTRUNNER__
#define __TESTRUNNER__

#import <MAUtil/String.h>

// ********** Class TestRunner **********

// Class that runs tests.
class TestRunner
{
public:
	// Methods.
	TestRunner();
	virtual ~TestRunner();
	virtual void reportResult();
	virtual void registerTestResult(
		bool testPassed,
		MAUtil::String testName,
		MAUtil::String testLabel);

protected:
	// Instance variables.
	int mPassedTests;
	int mFailedTests;

public:
	// Class variable to hold the global instance
	// of this class.
	static TestRunner* mInstance;

};

// ********** Macros **********

// Macros to make writing tests easier.

#define TestCase(name) \
	 MAUtil::String __testName(name)

#define ShouldHold(cond) \
	TestRunner::mInstance->registerTestResult(cond, __testName, #cond)

#endif
