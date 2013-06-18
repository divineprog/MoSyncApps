/*
Copyright (C) 2013 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/*
* @file Test.h
* @brief Testing framework
*/

#include <conprint.h>
#include <matime.h>
#include <mavsprintf.h>
#include "Test.h"

namespace MATest
{
	using namespace MAUtil;

	/* ========== Class TestListener ========== */

	TestListener::TestListener() {}
	TestListener::~TestListener() {}
	void TestListener::beginTestSuite(const String& suiteName) {}
	void TestListener::endTestSuite() {}
	void TestListener::beginTestCase(const String& testCaseName) {}
	void TestListener::endTestCase() {}
	void TestListener::assertion(const String& assertionName, bool cond) {}
	void TestListener::expectation(const String& assertionName) {}
	void TestListener::timedout(const String& testCaseName) {}


	/* ========== Class TestCaseTimeOutListener ========== */

	void TestCaseTimeOutListener::setTestCase(TestCase* testCase)
	{
		mTestCase = testCase;
	}

	void TestCaseTimeOutListener::startTimer(int ms)
	{
		stopTimer();
		MAUtil::Environment::getEnvironment().addTimer(this, ms, 1);
	}

	void TestCaseTimeOutListener::stopTimer()
	{
		MAUtil::Environment::getEnvironment().removeTimer(this);
	}

	void TestCaseTimeOutListener::runTimerEvent()
	{
		// The test case has timed out.
		stopTimer();
		testCase->timeOut();
	}

	/* ========== Class TestCase ========== */

	TestCase::TestCase(const String& name) :
		name(name)
	{
		mTimeOutListener.setTestCase(this);
	}

	TestCase::~TestCase()
	{
	}

	void TestCase::setTimeOut(int ms)
	{
		mTimeOutListener.startTimer(ms);
	}

	void TestCase::clearTimeOut()
	{
		mTimeOutListener.stopTimer();
	}

	void TestCase::open()
	{
	}

	void TestCase::close()
	{
	}

	bool TestCase::assert(const String& assertionName, bool cond)
	{
		suite->fireAssertion(assertionName, cond);
		return cond;
	}

	void TestCase::expect(const String& assertionName)
	{
		suite->fireExpectation(assertionName);
	}

	void TestCase::timeOut()
	{
		suite->fireTimedOut(name);
		runNextTestCase();
	}

	String TestCase::getName() const
	{
		return name;
	}

	void TestCase::setSuite(TestSuite *suite)
	{
		this->suite = suite;
	}

	TestSuite* TestCase::getSuite()
	{
		return this->suite;
	}

	void TestCase::runNextTestCase()
	{
		getSuite()->runNextCase();
	}

	/* ========== Class TestSuite ========== */

	TestSuite::TestSuite(const String& name) :
		mName(name),
		mCurrentTestCase(0),
		mRunCounter(0),
		mTestCaseDefaultTimeOut(20000) // 20 seconds
	{
	}

	TestSuite::~TestSuite()
	{
	}

	void TestSuite::addTestCase(TestCase* testCase)
	{
		mTestCases.add(testCase);
		testCase->setSuite(this);
	}

	void TestSuite::setTestCaseDefaultTimeout(int ms)
	{
		mTestCaseDefaultTimeOut = ms;
	}

// TODO: Why is this commented out?
// This code seems to run all test cases sequentially.
#if 0
	void TestSuite::runTestCases() {
		for(int j = 0; j < mTestListeners.size(); j++) {
			mTestListeners[j]->beginTestSuite(mName);
		}
		for(int i = 0; i < mTestCases.size(); i++) {
			for(int j = 0; j < mTestListeners.size(); j++) {
				mTestListeners[j]->beginTestCase(mTestCases[i]->getName());
			}
			mTestCases[i]->open();
			mTestCases[i]->run();
			mTestCases[i]->close();
			for(int j = 0; j < mTestListeners.size(); j++) {
				mTestListeners[j]->endTestCase();
			}
		}
		for(int j = 0; j < mTestListeners.size(); j++) {
			mTestListeners[j]->endTestSuite();
		}
	}
#endif

	// Note: This method is called from TestCase::runNextTestCase().
	void TestSuite::runNextCase()
	{
		// If this the number of waiting test cases is zero
		// we launch a timer to run the next test case.
		if (0 == mRunCounter)
		{
			MAUtil::Environment::getEnvironment().addTimer(this, 1, 1);
		}

		// Increment counter for test cases to run.
		mRunCounter ++;
	}

	void TestSuite::runTimerEvent()
	{
		// Remove the timer.
		MAUtil::Environment::getEnvironment().removeTimer(this);

		// Run next test case.
		runNextCaseHelper();

		// Decrement run counter.
		mRunCounter --;

		// If there are test cases left to run, start a timer
		// to run it/them (should likely be only one waiting).
		if (mRunCounter > 0)
		{
			MAUtil::Environment::getEnvironment().addTimer(this, 1, 1);
		}
	}

	void TestSuite::runNextCaseHelper()
	{
		TestCase* testCase;

		// There must be test cases to run.
		if (mTestCases.size() > 0)
		{
			// Is this the first test case?
			if (0 == mCurrentTestCase)
			{
				// Signal beginning of the test suite.
				fireBeginTestSuite(mName);
			}

			// If last test case has been run, reset the
			// test case index and end the suite.
			if (mCurrentTestCase >= mTestCases.size())
			{
				mCurrentTestCase = 0;
				fireEndTestSuite();
				return;
			}

			// If this is not the first test case then
			// signal the end of the previous test case.
			if (mCurrentTestCase > 0)
			{
				// Close/clear the current test case.
				testCase = mTestCases[mCurrentTestCase];
				testCase->clearTimeOut();
				testCase->close();
				fireEndTestCase();
			}

			// Get the current test case.
			testCase = mTestCases[mCurrentTestCase];

			// Increment test case index.
			mCurrentTestCase ++;

			// Open the current test case.
			fireBeginTestCase(testCase->getName());
			testCase->open();

			// Run current test case.
			testCase->setTimeOut(mTestCaseDefaultTimeOut);
			testCase->start();

			// Note: We do not call fireEndTestCase() here,
			// because it should not be called until the
			// current test case has called runNextTestCase().
		}
	}

	const String& TestSuite::getName() const
	{
		return mName;
	}

	void TestSuite::addTestListener(TestListener* testListener)
	{
		mTestListeners.add(testListener);
	}

	void TestSuite::fireBeginTestSuite(const String& suiteName)
	{
		for (int j = 0; j < mTestListeners.size(); j++)
		{
			mTestListeners[j]->beginTestSuite(suiteName);
		}
	}

	void TestSuite::fireEndTestSuite()
	{
		for (int j = 0; j < mTestListeners.size(); j++)
		{
			mTestListeners[j]->endTestSuite();
		}
	}

	void TestSuite::fireBeginTestCase(const String& testCaseName)
	{
		for (int j = 0; j < mTestListeners.size(); j++)
		{
			mTestListeners[j]->beginTestCase(testCaseName);
		}
	}

	void TestSuite::fireEndTestCase()
	{
		for (int j = 0; j < mTestListeners.size(); j++)
		{
			mTestListeners[j]->endTestCase();
		}
	}

	void TestSuite::fireAssertion(const String& assertionName, bool cond)
	{
		for (int j = 0; j < mTestListeners.size(); j++)
		{
			mTestListeners[j]->assertion(assertionName, cond);
		}
	}

	void TestSuite::fireExpectation(const String& assertionName)
	{
		for (int j = 0; j < mTestListeners.size(); j++)
		{
			mTestListeners[j]->expectation(assertionName);
		}
	}
}
// namespace
