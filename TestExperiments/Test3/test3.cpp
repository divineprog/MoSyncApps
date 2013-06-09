
// **********************************
// ********** TestRunner.h **********
// **********************************

#ifndef __TESTRUNNER_H
#define __TESTRUNNER_H

#include <stdio.h>
#include <vector>

#define TESTCASE(fun) registerTestCase(this, (TestCaseFun)&fun, #fun)
#define ASSERT(cond) assert(#cond, cond)
#define EXPECT(name) expect(name)
#define TEST(testClass) testClass __testClass; __ ## testClass .setTestName(#testClass)

/*
 Terminology:

 TestRunner
   has one TestLogic
   has many Test

   TestSuite
     Test
       TestCase
         Expect/Assert
*/


// ********** Class TestRunner **********

class Test;
class TestLogic;

class TestRunner
{
public:
    // Single instance of TestRunner.
    static TestRunner* mInstance;
    
    // Vector that holds Test objects.
    std::vector<Test*> mTests;
    
    // Pluggable object that defines central test logic:
    // assert/expect, log and report test results.
    TestLogic* mTestLogic;
    
    TestRunner();
    
    void addTest(Test* test);
    
    void setTestLogic(TestLogic* testLogic);
};

// ********** Class TestLogic **********

class TestLogic
{
public:
    TestRunner* mTestRunner;
    
    TestLogic(TestRunner* testRunner);
    
    virtual void runAllTests();
    
    virtual void assert(const char* testName, const char* assertName, bool cond);
    
    virtual void expect(const char* testName, const char* assertName);
};

// ********** Class Test **********

class Test
{
public:
    typedef void (Test::*TestCaseFun)(void);
    
    // Name of test.
    const char* mTestName;
    
    // Vector that holds test case functions.
    std::vector<TestCaseFun> mTestCaseFuns;
    
    Test(const char* testName);
    
    virtual void setTestName(const char* testName);
    
    virtual void registerTestCases() {}
    
    virtual void registerTestCase(Test* test, TestCaseFun fun, const char* testCaseName);
    
    virtual void runAllTestCases();
    
    virtual void assert(const char* assertName, bool cond);
    
    virtual void expect(const char* assertName);
};

#endif // __TESTRUNNER_H

// ************************************
// ********** TestRunner.cpp **********
// ************************************

TestRunner::TestRunner()
{
}

void TestRunner::addTest(Test* test)
{
    mTests.push_back(test);
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

void TestLogic::runAllTests()
{
    for (int i = 0; i < mTestRunner->mTests.size(); ++i)
    {
        Test* test = mTestRunner->mTests[i];
        test->runAllTestCases();
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

// ********** Class Test **********

Test::Test(const char* testName)
{
    printf("Hello Test\n");
    mTestName = testName;
}

void Test::setTestName(const char* testName)
{
    mTestName = testName;
}

void Test::registerTestCase(Test* test, TestCaseFun fun, const char* testCaseName)
{
    printf("regTestCase %s\n", testCaseName);
    //(test->*fun)();
    mTestCaseFuns.push_back(fun);
}

void Test::runAllTestCases()
{
    for (int i = 0; i < mTestCaseFuns.size(); ++i)
    {
        TestCaseFun testCaseFun = mTestCaseFuns[i];
        (this->*testCaseFun)();
    }
}

void Test::assert(const char* assertName, bool cond)
{
    TestRunner::mInstance->mTestLogic->assert(mTestName, assertName, cond);
}

void Test::expect(const char* assertName)
{
    TestRunner::mInstance->mTestLogic->expect(mTestName, assertName);
}
    
// ********************************
// ********** MyTest.cpp **********
// ********************************

class MyTest : public Test
{
public:
    MyTest(const char* testName) : Test(testName)
    {
        printf("Hello MyTest\n");
        //registerTestCase(this, (TestCaseFun)&MyTest::sendToBg, "MyTest::sendToBg");
        TESTCASE(MyTest::sendToBg);
    }
    
    void sendToBg()
    {
        printf("sendToBg\n");
    }
};
//TEST(MyTest);
MyTest __MyTest("MyTest");


// ******************************
// ********** main.cpp **********
// ******************************

int main()
{
    printf("Hello main\n");
    
    TestLogic* testLogic = new TestLogic(TestRunner::mInstance);
    (TestRunner::mInstance)->setTestLogic(testLogic);
    (TestRunner::mInstance)->mTestLogic->runAllTests();
}
