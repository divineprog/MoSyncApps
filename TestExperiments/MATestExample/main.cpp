#include <MAUtil/Moblet.h>
#include <MAUtil/String.h>
#include <conprint.h>
#include "TestRunner.h"

using namespace MAUtil;

// Declare entry point for running tests.
// Add more functions here as needed.
void RunAllTests();

class MyMoblet : public Moblet
{
public:
	MyMoblet()
	{
		printf("MATestExample\n");
		printf("Press zero or back to exit\n");

		// Set test listener that will report results.
		TestRunner::getInstance()->addTestListener(
			new SimpleTestListener());

		// Run tests.
		TestRunner::getInstance()->runNextTestCase();
	}

	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Call close to exit the application.
			close();
		}
	}
};

extern "C" int MAMain()
{
	Moblet::run(new MyMoblet());
	return 0;
}
