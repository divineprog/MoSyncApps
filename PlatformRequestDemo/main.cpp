/**
 * @file main.cpp
 *
 * This template application shows how to call maPlatformRquest with
 * parameters passed from JavaScript.
 */

#include <Wormhole/HybridMoblet.h>

#include "MAHeaders.h" // Defines BEEP_WAV

// Namespaces we want to access.
using namespace MAUtil;
using namespace NativeUI;
using namespace Wormhole;

/**
 * The application class.
 */
class MyMoblet : public HybridMoblet
{
public:
	/**
	 * Constructor.
	 */
	MyMoblet()
	{
		// Show the start page.
		showPage("index.html");

		// Set the sound used by the PhoneGap beep notification API.
		// BEEP_WAV is defined in file Resources/Resources.lst.
		// Below we add our own beep message, to illustrate how to
		// invoke custom C++ code from JavaScript. Do not confuse these
		// two ways of playing a beep sound.
		setBeepSound(BEEP_WAV);

		// Register function to handle messages from JavaScript.
		addMessageFun(
			"PlatformRequest",
			(FunTable::MessageHandlerFun)&MyMoblet::handlePlatformRequest);
	}

	/**
	 * Function that opens the requested URL.
	 */
	void handlePlatformRequest(Wormhole::MessageStream& message)
	{
		// Obtain URL parameter sent from JavaScript.
		const char* url = message.getNext();

		// Make the request to open the URL.
		maPlatformRequest(url);
	}
};

/**
 * Main function that is called when the program starts.
 * Here an instance of the MyMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain()
{
	(new MyMoblet())->enterEventLoop();
	return 0;
}
