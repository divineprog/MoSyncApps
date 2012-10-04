
/**
 * @file main.cpp
 * @author Ali Sarrafi & Mikael Kindborg
 *
 * Application that reads Twitter flows.
 *
 * This program illustrates how to use the jQTouch JavAScript
 * library in a MoSync WebView application. The program also
 * shows how to communicate from JavaScript to C++.
 *
 * The entire UI and most of the application logic is implemented
 * in JavaScript.
 */

#include <Wormhole/WebAppMoblet.h>
#include <Wormhole/MessageProtocol.h>
#include <Wormhole/MessageStream.h>
#include <Wormhole/Libs/JSONMessage.h>
#include <Wormhole/Libs/PhoneGap/PhoneGapMessageHandler.h>
#include <Wormhole/Libs/JSNativeUI/NativeUIMessageHandler.h>
#include <Wormhole/Libs/JSNativeUI/ResourceMessageHandler.h>

#include <CustomWebAppMoblet.h>

#include "FileMessageHandler.h" // Custom File API.

#include "MAHeaders.h"

// Namespaces we want to access.
using namespace MAUtil; // Class Moblet
using namespace NativeUI; // WebView widget.
using namespace Wormhole; // Wormhole library.

/**
 * The application class.
 */
class TwitterMoblet : public CustomWebAppMoblet
{
public:
	TwitterMoblet()
	{
		// Show the start page. This will also perform initialization if needed.
		showPage("index.html");

		// TODO: Call from JavaScript last in index.html, or last in wormhole.js ?
		// Have written code in MessageHandler.cpp to call initializePhoneGap().
		// Problem with having it here may be that PhoneGap is not loaded
		// when initializePhoneGap() is called (is this possible?).
		initializePhoneGap();

		// The beep sound is defined in file "Resources/Resources.lst".
		setBeepSound(BEEP_WAV);

		// Turn off zoom.
		getWebView()->disableZoom();

		// Register functions to handle custom messages sent from JavaScript.
		addMessageFun(
			"File",
			(FunTable::MessageHandlerFun)&MyMoblet::handleFileMessage);
	}

	virtual ~TwitterMoblet()
	{
		// Add cleanup code as needed.
	}

	void handleFileMessage(Wormhole::MessageStream& stream)
	{
		mMessageHandler.handleMessage(message);
	}

private:
	/**
	 * Object that handles messages from JavaScript.
	 */
	FileMessageHandler mMessageHandler;
};

/**
 * Main function that is called when the program starts.
 * Here an instance of the MyMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain()
{
	Moblet::run(new TwitterMoblet());
	return 0;
}
