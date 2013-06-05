#ifndef __TESTRUNNER__
#define __TESTRUNNER__

#import <MAUtil/String.h>
#import <MAUtil/Vector.h>

// Forward declaration.
class Test;

// Function type used for tests.
typedef void (*TestFun)(Test*);

// ********** Class TestRunner **********

// Class that runs tests.
class TestRunner
{
public:
	// Methods.
	TestRunner();
	virtual ~TestRunner();
	virtual void addTest(Test* test);
	virtual void runAllTests();
	virtual void reportResult();
	virtual void registerTestResult(
		bool testPassed,
		MAUtil::String testName,
		MAUtil::String testLabel);

protected:
	// Instance variables.
	MAUtil::Vector<Test*> mTests;
	int mPassedTests;
	int mFailedTests;

public:
	// Class variable to hold the global instance
	// of this class.
	static TestRunner* mInstance;

};

// ********** Class Test **********

// Class that represents a test.
class Test
{
public:
	Test(TestFun testFun, MAUtil::String testName);
	void shouldHold(bool cond, MAUtil::String label);

	TestFun mTestFun;
	MAUtil::String mTestName;
};

// ********** Macros **********

// Macros to make writing tests easier.

#define TestCase(fun) \
	void fun(Test* __test)

#define TestCaseEnd(fun) \
	Test __##fun(fun, #fun);

#define ShouldHold(cond) \
	__test->shouldHold(cond, #cond)

#endif
