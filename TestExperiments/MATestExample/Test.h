#ifndef __TEST_H_
#define __TEST_H_

#include <ma.h>
#include <MAUtil/Vector.h>
#include <MAUtil/String.h>

/**
* \brief MoSync test case classes
*/
namespace MATest {

	// TODO: Yuck! Do not use namespaces in header files!!
	using namespace MAUtil;

	/**
	* \brief Listener for events triggered by the tests
	*/
	class TestListener {
	public:
		virtual void beginTestSuite(const String& suiteName);
		virtual void endTestSuite();
		virtual void beginTestCase(const String& testCaseName);
		virtual void endTestCase();
		virtual void assertion(const String& assertionName, bool cond);
		virtual void expect(const String& assertionName);
		virtual ~TestListener();
	};

	class TestSuite;

	/**
	* \brief A test case.
	*
	* Implement this class for each of your test cases. Use assert when testing
	* assertions. Override open and close to initialize and deinitialize test case
	* specific things. Override start for the tests.
	*
	* Do not initialize any non-const variables in the constructor,
	* as the test may be run more than once.
	*/
	class TestCase {
	public:
		TestCase(const String& name);
		virtual ~TestCase();

		virtual void open();
		virtual void close();
		virtual void start() = 0;

		void addTestListener(TestListener *testListener);
		bool assert(const String& assertionName, bool success);
		void expect(const String& assertionName);
		const String& getName() const;
		void setSuite(TestSuite *suite);
		TestSuite* getSuite();
		void runNextTestCase();

	protected:
		TestSuite *suite;
		String name;
	};

	/**
	* \brief A list of TestCases.
	*
	* Add test cases with addTestCase.
	* Use runTestCases to run all test cases.
	*/
	class TestSuite {
	public:
		TestSuite(const String& name);
		~TestSuite();
		void addTestCase(TestCase *testCase);
		void runNextCase();
		const String& getName() const;
		void addTestListener(TestListener *testListener);
		void fireAssertion(const String& str, bool success);
		void fireExpect(const String& str);
	private:
		String name;
		Vector<TestCase*> testCases;
		Vector<TestListener*> testListeners;
		int mNextCase;
	};

}

#endif	//_SE_MSAB_TEST_H_
