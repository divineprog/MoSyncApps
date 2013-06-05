#include <MAUtil/Moblet.h>
#include <MAUtil/String.h>
#include <conprint.h>
#include "TestRunner.h"

using namespace MAUtil;

class MyMoblet : public Moblet
{
public:
	MyMoblet()
	{
		printf("Running tests\n");

		// Run tests and report the final result.
		TestRunner::mInstance->runAllTests();
		TestRunner::mInstance->reportResult();

		printf("Press zero or back to exit\n");
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
