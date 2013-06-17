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

#ifndef __MOSYNC_LIBS_MATEST_TEST_H__
#define __MOSYNC_LIBS_MATEST_TEST_H__

#include <ma.h>
#include <MAUtil/Vector.h>
#include <MAUtil/String.h>

namespace MATest
{
	/**
	* @brief Listener for events triggered when running tests.
	*/
	class TestListener
	{
	public:
		/**
		 * @brief Constructor.
		 */
		TestListener();

		/**
		 * @brief Destructor.
		 */
		virtual ~TestListener();

		/**
		 * @brief Called when starting a test suite.
		 */
		virtual void beginTestSuite(const MAUtil::String& suiteName);

		/**
		 * @brief Called when ending a test suite.
		 */
		virtual void endTestSuite();

		/**
		 * @brief Called when starting a test case.
		 */
		virtual void beginTestCase(const MAUtil::String& testCaseName);

		/**
		 * @brief Called when ending a test case.
		 */
		virtual void endTestCase();

		/**
		 * @brief Called when an assert is made.
		 */
		virtual void assertion(const MAUtil::String& assertName, bool cond);

		/**
		 * @brief Called when an expect is made.
		 * An expect says that "this assert is
		 * expected to happen in the future".
		 */
		virtual void expectation(const MAUtil::String& assertName);
	};

	// Forward declaration.
	class TestSuite;

	/**
	* @brief Base class for test cases.
	*
	* Override open and close to initialize and clean up the
	* test case as needed. Override start to perform actual tests.
	* Use assert state conditions in the test that should hold.
	*
	* Do not initialize any non-const variables in the constructor,
	* as the test case might be run more than once.
	*/
	class TestCase
	{
	public:
		/**
		 * @brief Constructor.
		 */
		TestCase(const MAUtil::String& name);

		/**
		 * @brief Destructor.
		 */
		virtual ~TestCase();

		/**
		 * @brief Called before a test case is run.
		 * Do initialisation here as needed.
		 */
		virtual void open();

		/**
		 * @brief Called after a test case is run.
		 * Do clean up here as needed.
		 */
		virtual void close();

		/**
		 * @brief Called to run a test case.
		 * You must override this method and
		 * perform tests in a subclass.
		 */
		virtual void start() = 0;

		/**
		 * @brief Test a condition that should hold.
		 */
		virtual bool assert(const MAUtil::String& assertionName, bool success);

		/**
		 * @brief State that an assert is expected to happen
		 * in the future.
		 */
		virtual void expect(const MAUtil::String& assertionName);

		/**
		 * @brief Call to run the next test case. This must
		 * be called at the end of each test case, during the
		 * invocation of the start method.
		 */
		virtual void runNextTestCase();

		/**
		 * @brief Get the name of this test case.
		 */
		virtual const MAUtil::String& getName() const;

		/**
		 * @brief Get the test suite this test case belongs to.
		 */
		virtual TestSuite* getSuite();

		/**
		 * @brief Set the test suite of this test case.
		 * You normally do not call this method explicitly,
		 * it is used by the test framework.
		 */
		virtual void setSuite(TestSuite* suite);

	protected:
		/**
		 * @brief @deprecated Do not access this variable in your code!
		 * Use getSuite() instead.
		 */
		TestSuite* suite;

		/**
		 * @brief @deprecated Do not access this variable in your code!
		 * Use getName() instead.
		 */
		MAUtil::String name;
	};

	/**
	* @brief A test suite holds a list of test cases.
	*/
	class TestSuite {
	public:
		/**
		 * @brief Constructor.
		 */
		TestSuite(const MAUtil::String& name);

		/**
		 * @brief Destructor.
		 */
		virtual ~TestSuite();

		/**
		 * Add a test case to the suite.
		 */
		virtual void addTestCase(TestCase* testCase);

		/**
		 * Run the next test case.
		 */
		virtual void runNextCase();

		/**
		 * Get the name of the suite.
		 */
		virtual const MAUtil::String& getName() const;

		/**
		 * Add listener that will be notified of test results.
		 */
		virtual void addTestListener(TestListener* testListener);

		/**
		 * Send test suite begin to listeners.
		 */
		virtual void fireBeginTestSuite(const MAUtil::String& suiteName);

		/**
		 * Send test suite end to listeners.
		 */
		virtual void fireEndTestSuite();

		/**
		 * Send test case begin to listeners.
		 */
		virtual void fireBeginTestCase(const MAUtil::String& testCaseName);

		/**
		 * Send test case end to listeners.
		 */
		virtual void fireEndTestCase();

		/**
		 * Send assertion to listeners.
		 */
		virtual void fireAssertion(const MAUtil::String& assertionName, bool success);

		/**
		 * Send expectation to listeners.
		 */
		virtual void fireExpectation(const MAUtil::String& assertionName);

	protected:
		/**
		 * Name of test suite.
		 */
		MAUtil::String mName;

		/**
		 * List of test cases.
		 */
		MAUtil::Vector<TestCase*> mTestCases;

		/**
		 * List of test listeners.
		 */
		MAUtil::Vector<TestListener*> mTestListeners;

		/**
		 * Index that point sto the current test case.
		 */
		int mCurrentTestCase;

		/**
		 * Has method runNextCase been called?
		 */
		bool mRunNextCaseCalled;

		/**
		 * Is the current test case running asynchronously?
		 */
		bool mIsRunningAsync;
	};

} // namespace

#endif
