# Test Experiments

## Overview

This folder contains two projects:

* TestRunner - Uses function pointers and helper classes to register tests. Tests are registered where they are defined in the source code. This is done by creating a class that adds a function pointer to the list of tests. The test runner is called in main.cpp.

* TestRunnerSimple - Uses plain functions for tests, which are manually added to a main test runner function which in turn is called from main.cpp. This approach is "simpler" because the test framework is smaller.

Both examples use function pointers for test cases. Another option is to make test cases classes. This provides  additional capabilities in that classes can hold many objects, maintain state, implement listeners, etc. MATest uses this approach.

For short test cases, functions are convinient, for more complext test cases, classes are propably a better option. The question is which kinds of tests will be most common? I guess a function can also just use a class to create an object for the test logic, so functions doe not exclude the use of classes.

Both examples currently have a problem, you cannot plug in a sublcass of TestRunner and instantiate it without editing the code in TestRunner.cpp. This can be fixed by adding another pluggable class, that is called for methods like registerTestResult, and reportResult. This would be very useful as it can be used to configure where tests results should be reported: on the display, saved in a file, or sent over the network.

There is also no support for asynchronous tests, and this should be added (see discussion below).

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

## Requirements discussion

Basic capabilities required:

* Run tests: Tests can be run explicitly (as in TestRunnerSimple) or be registered "automatically" (as in TestRunner). Perhaps it is good to support both ways.

* Log test results: A test framework is esentially a logging framework. Results could be reported as tests (asserts) are made, or collected and reported at the end of the test.

* Report test results: The framework needs a way to report a summary of the logged results. Results could be displayed on the display, written to a file, or sent to a server. It should be possible to plugin custom behaviour for this.

* Async tests: It should be possible to test event-based systems, where results are obtained asynchronously.

## Async tests

For async tests, it can be useful to have the ability to state that a certain condition/state is to be expected in the future. The test framework would then report a failure if the expected condition is not met within a given timeout.

It can also be useful to be able to wait with running next test until the currently running test is completed. For example, in a sequence of downloader test, one may not wish to start next test until the first one is completed, because then tests may interfer. It may also be that case that one do want downloder tests running in parallel, to test that case.

Here is a code sketch for async tests. The style used in this example is that a test (test case) is identified by a string. This is what the test logger (TestRunner) uses.

Defining tests:

    #include "TestRunner.h"

    void PlainTest(Test* test)
    {
        test->assert("1==1", 1==1);
    }
    
    void DownloaderTest1(Test* test)
    {
        // Tells test system to wait with running next test
        // until this test is done.
        test->waitUntilDone();
        
        // Tells tests sytem we expect this assert to happen
        // and it is a faiure if it does not happen.
        test->expect("DownloadFinished");
        
        // Some code for starting a downloader.
        startDownloading1();
    }
    
    // DownloaderTest1 downloader listener.
	void finishedDownloading(MAUtil::Downloader* downloader, MAHandle data)
    {
		Test* test = TestRunner::getTest("DownloaderTest1");
		test->assert("DownloadFinished", data > 0);
		test->done();
	}
    
    // Will run download in parallel with DownloaderTest3.
    void DownloaderTest2(Test* test)
    {
        // Tells tests sytem we expect this assert to happen
        // and it is a faiure if it does not happen.
        test->expect("DownloadFinished");
        
        // Some code for starting a downloader.
        startDownloading2();
    }
    
    // DownloaderTest2 downloader listener.
	void finishedDownloading(MAUtil::Downloader* downloader, MAHandle data)
    {
		Test* test = TestRunner::getTest("DownloaderTest2");
		test->assert("DownloadFinished", data > 0);
	}
    
    // Will run download in parallel with DownloaderTest2.
    void DownloaderTest3(Test* test)
    {
        // Tells tests sytem we expect this assert to happen
        // and it is a faiure if it does not happen.
        test->expect("DownloadFinished");
        
        // Some code for starting a downloader.
        startDownloading3();
    }
    
    // DownloaderTest3 downloader listener.
	void finishedDownloading(MAUtil::Downloader* downloader, MAHandle data)
    {
		Test* test = TestRunner::getTest("DownloaderTest3");
		test->assert("DownloadFinished", data > 0);
	}
    
    // Declare test cases.
    TestCase(PlainTest) // Expands to: Test __PlainTest(PlainTest, "PlainTest");
    TestCase(DownloaderTest1)
    TestCase(DownloaderTest2)
    TestCase(DownloaderTest3)
    
Running tests:

    // Run tests and report the final result.
    TestRunner::runAllTests();
    
    // Wait for tests to complete (for all expect calls to have a 
    // corresponding assert). Timeout after 20 seconds.
    TestRunner::waitForTestsToComplete(20000);
    
    // Report the result.
    TestRunner::reportResult();

The idea here is that TestRunner is a singleton where you can plugin your own class to handle test logging/reporting. For example:

    TestRunner::setLogger(new MyTestLogger());
    
Example of interface:

    class TestLogger
    {
        virtual void logTestResult(String testName, String assertName, bool cond) = 0;
        virtual void reportTestResult() = 0;
    };

What I do like about MATest is that there are no macros, and no global objects (singletons). This makes for a clean code structure with few assumptions.
