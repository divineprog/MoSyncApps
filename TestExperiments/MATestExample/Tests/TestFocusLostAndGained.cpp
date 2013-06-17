#include <ma.h>
#include <MAUtil/Environment.h>
#include "../TestRunner.h"

/**
 * This example does not work very well, so it
 * is currently not run as part of the tests.
 *
 * This test works  badly on Android, the app goes
 * to background, but is then restarted when opened
 * again from the home screen. Expected the app to
 * be opened in its previous state.
 *
 * Furthermore maBringToForeground does not work on Android,
 * at least not in this test.
 */
class TestFocusLostAndGained :
	public MATest::TestCase,
	public MAUtil::FocusListener
{
public:
	TestFocusLostAndGained(const MAUtil::String& name)
		: MATest::TestCase(name)
	{
	}

	void start()
	{
		// Register focus listener.
		MAUtil::Environment::getEnvironment()
			.addFocusListener(this);

		expect("FocusLost");

		maSendToBackground();
	}

	void focusLost()
	{
		// Remove focus listener.
		MAUtil::Environment::getEnvironment()
			.removeFocusListener(this);

		assert("FocusLost", true);

		runNextTestCase();
	}

	void focusGained()
	{
		// maBringToForeground does not seem to work on Android.
		// No point in testing this case.
	}

	MAUtil::FocusListener* mFocusListener;
};

// Commented out test that works badly.
// TESTCASE(TestFocusLostAndGained)
