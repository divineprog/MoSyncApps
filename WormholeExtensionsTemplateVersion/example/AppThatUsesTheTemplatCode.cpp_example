/**
 * @file main.cpp
 *
 * This template application shows how to extend the functionality
 * in HTML5/JS with custom code written in C++. See code below
 * for custom code added in methods handleMessageStreamJSON()
 * and handleMessageStream().
 *
 * When reading the code below, it is good to know that there are
 * two message formats: JSON and string streams. String streams are
 * generally faster. See comments in the code below for further details.
 * PhoneGap uses JSON messages, NativeUI uses string streams.
 */

#include <Wormhole/WebAppMoblet.h>
#include <Wormhole/MessageProtocol.h>
#include <Wormhole/MessageStream.h>
#include <Wormhole/Libs/JSONMessage.h>
#include <Wormhole/Libs/PhoneGap/PhoneGapMessageHandler.h>
#include <Wormhole/Libs/JSNativeUI/NativeUIMessageHandler.h>
#include <Wormhole/Libs/JSNativeUI/ResourceMessageHandler.h>

#include <CustomWebAppMoblet.h>
#include <FunTable.h>

#include "MAHeaders.h"

// Namespaces we want to access.
using namespace MAUtil; // Class Moblet
using namespace NativeUI; // WebView widget.
using namespace Wormhole; // Wormhole library.

/**
 * The application class.
 */
class MyMoblet : public CustomWebAppMoblet
{
public:
	MyMoblet()
	{
		init(BEEP_WAV);

		// Register functions to handle custom messages sent from JavaScript.
		addMessageFun(
			"Vibrate",
			(FunTable<WebAppMoblet>::MessageHandlerFun)&MyMoblet::vibrate);

		// The page in the "LocalFiles" folder to
		// show when the application starts.
		showPage("index.html");
	}

	virtual ~MyMoblet()
	{
		// Add cleanup code as needed.
	}

	void vibrate(Wormhole::MessageStream& stream)
	{
		maVibrate(500);
	}
};

/**
 * Main function that is called when the program starts.
 * Here an instance of the MyMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain()
{
	Moblet::run(new MyMoblet());
	return 0;
}
