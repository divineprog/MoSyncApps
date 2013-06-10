#include <ma.h>
#include <MAUtil/Environment.h>
#include "TestRunner.h"

// This is nasty, but to was the most simple thing.
static MAUtil::FocusListener* sFocusListener;

class MyFocusListener : public MAUtil::FocusListener
{
public:
	MATest::TestCase* mTestCase;

	MyFocusListener(MATest::TestCase* testCase)
	{
		mTestCase = testCase;
	}

	void focusLost()
	{
		// Remove focus listener.
		MAUtil::Environment::getEnvironment()
			.removeFocusListener(sFocusListener);

		mTestCase->assert("FocusLost", true);
		mTestCase->runNextTestCase();
	}

	void focusGained()
	{
		// maBringToForeground does not seem to work on Android.
		// No point in testing this case.
	}
};

/**
 * This is just a very simple example of
 * a test case that demonstrates the use
 * of plain asserts (no async calls etc).
 */
class TestFocusLostAndGained : public MATest::TestCase
{
public:
	TestFocusLostAndGained(const MAUtil::String& name)
		: MATest::TestCase(name)
	{
	}

	void start()
	{
		sFocusListener = new MyFocusListener(this);

		// Register focus listener.
		MAUtil::Environment::getEnvironment()
			.addFocusListener(sFocusListener);

		expect("FocusLost");

		maSendToBackground();
	}
};

// Commented out test that works badly.
// TESTCASE(TestFocusLostAndGained)

// This test works kind of badly on Android,
// the app goes to background, but is then restarted
// when opened again from the home screen. Expected app
// to be opened in its previous state.
// Furthermore maBringToForeground does not work on Android,
// at least not in this test.

