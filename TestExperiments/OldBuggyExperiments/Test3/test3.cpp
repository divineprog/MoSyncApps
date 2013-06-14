
// **********************************
// ********** TestRunner.h **********
// **********************************

#ifndef __TESTRUNNER_H
#define __TESTRUNNER_H

#include <stdio.h>
#include <vector>

#define DEF_TEST(fun) registerTestCase((TestCaseFun)&fun, #fun)
#define DET_TESTSUITE(testSuiteClass) testSuiteClass __testSuiteClass;

/*
 Terminology/hierarchy:

 TestRunner
   has one TestLogic
   has many TestSuite
     has many Test
       has many Expect/Assert
*/


// ********** Class TestRunner **********

class TestLogic;
class TestSuite;
class TestCase;

class TestRunner
{
public:
    // Single instance of TestRunner.
    static TestRunner* mInstance;
    
    // Vector that holds TestSuite objects.
    std::vector<TestSuite*> mTestSuites;
    
    // Pluggable object that defines central test logic:
    // assert/expect, log and report test results.
    TestLogic* mTestLogic;
    
    TestRunner();
    
    void addTestSuite(TestSuite* suite);
    
    void setTestLogic(TestLogic* testLogic);
};

// ********** Class TestLogic **********

class TestLogic
{
public:
    TestRunner* mTestRunner;
    
    TestLogic(TestRunner* testRunner);
    
    virtual void runAllTestSuites();
    
    virtual void assert(const char* testCaseName, const char* assertName, bool cond);
    
    virtual void expect(const char* testCaseName, const char* assertName);
};

// ********** Class TestSuite **********

typedef void (TestSuite::*TestCaseFun)(TestCase& testCase);
    
class TestSuite
{
public:
    // Vector that holds test cases.
    std::vector<TestCase> mTestCases;
    
    TestSuite();
    
    virtual void registerTestCase(TestCaseFun fun, const char* testCaseName);
    
    virtual void runAllTestCases();
};

// ********** Class TestCase **********

class TestCase
{
public:

    TestCaseFun mTestCaseFun;
    const char* mTestCaseName;
    
    TestCase(TestCaseFun fun, const char* name);

    void assert(const char* assertName, bool cond);

    void expect(const char* assertName);
};

#endif // __TESTRUNNER_H

// ************************************
// ********** TestRunner.cpp **********
// ************************************

TestRunner::TestRunner()
{
}

void TestRunner::addTestSuite(TestSuite* suite)
{
    mTestSuites.push_back(suite);
}

void TestRunner::setTestLogic(TestLogic* testLogic)
{
    mTestLogic = testLogic;
}

TestRunner* TestRunner::mInstance = new TestRunner();

// ********** Class TestLogic **********

TestLogic::TestLogic(TestRunner* testRunner)
{
    mTestRunner = testRunner;
}

void TestLogic::runAllTestSuites()
{
    for (int i = 0; i < mTestRunner->mTestSuites.size(); ++i)
    {
        TestSuite* suite = mTestRunner->mTestSuites[i];
        suite->runAllTestCases();
    }
}

void TestLogic::assert(const char* testName, const char* assertName, bool cond)
{
    printf("%s assert: %s %i\n", testName, assertName, cond);
}

void TestLogic::expect(const char* testName, const char* assertName)
{
    printf("%s expect: %s\n", testName, assertName);
}

// ********** Class TestSuite **********

TestSuite::TestSuite()
{
    TestRunner::mInstance->addTestSuite(this);
}

void TestSuite::registerTestCase(TestCaseFun fun, const char* name)
{
    mTestCases.push_back(TestCase(fun, name));
}

void TestSuite::runAllTestCases()
{
    for (int i = 0; i < mTestCases.size(); ++i)
    {
        TestCase testCase = mTestCases[i];
        TestCaseFun fun = testCase.mTestCaseFun;
        (this->*fun)(testCase);
    }
}
    
// ********** Class TestCase **********

TestCase::TestCase(TestCaseFun fun, const char* name)
{
    mTestCaseFun = fun;
    mTestCaseName = name;
}

void TestCase::assert(const char* assertName, bool cond)
{
    TestRunner::mInstance->mTestLogic->assert(mTestCaseName, assertName, cond);
}

void TestCase::expect(const char* assertName)
{
    TestRunner::mInstance->mTestLogic->expect(mTestCaseName, assertName);
}

// *************************************
// ********** MyTestSuite.cpp **********
// *************************************

class MyTestSuite : public TestSuite
{
public:
    MyTestSuite()
    {
        //registerTestCase((TestFun)&MyTestSuite::sendToBg, "MyTestSuite::sendToBg");
        DEF_TEST(MyTestSuite::sendToBg);
    }
    
    void sendToBg(TestCase& t)
    {
        t.assert("Assert1", 1==1);
    }
};
//DEF_TESTSUITE(MyTestSuite);
MyTestSuite __MyTestSuite;


// ******************************
// ********** main.cpp **********
// ******************************

int main()
{
    printf("Hello main\n");
    
    TestLogic* testLogic = new TestLogic(TestRunner::mInstance);
    (TestRunner::mInstance)->setTestLogic(testLogic);
    (TestRunner::mInstance)->mTestLogic->runAllTestSuites();
}
