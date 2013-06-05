# Test Experiments

This folder contains two projects:

* TestRunner - Uses function pointers and helper classes to register tests. Tests are registered where they are defined in the source code. This is done by creating a class that adds a function pointer to the list of tests. The test runner is called in main.cpp.

* TestRunnerSimple - Uses plain functions for tests, which are manually added to a main test runner function which in turn is called from main.cpp. This approach is "simpler" because the test framework is smaller.

Both examples currently have a problem, you cannot plug in a sublcass of TestRunner and instantiate it without editing the code in TestRunner.cpp. This can be fixed by adding another pluggable class, that is called for methods like registerTestResult, and reportResult. This would be very useful as it can be used to configure where tests results should be reported: on the display, saved in a file, or sent over the network.

I broke all conventions and used mixed case macros because they look nicer.

## Code example for TestRunner

Sample test cases (see file TestRunner/MyTests.cpp):

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
    
Running tests (see file TestRunner/main.cpp):

		// Run tests and report the final result.
		TestRunner::mInstance->runAllTests();
		TestRunner::mInstance->reportResult();

## Code example for TestRunnerSimple

Sample test cases (see file TestRunnerSimple/MyTests.cpp):

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

Running tests (see file TestRunnerSimple/main.cpp):
    
    // Declare entry point for running tests.
    void RunAllTests();

		// Run tests and report the final result.
		RunAllTests();
		TestRunner::mInstance->reportResult();
