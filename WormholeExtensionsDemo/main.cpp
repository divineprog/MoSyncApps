/**
 * @file main.cpp
 *
 * This template application shows how to extend the functionality
 * in HTML5/JS with custom code written in C++. The code uses a new
 * class called CustomWebAppMoblet, which in turn uses classes
 * MessageHandler, and FunTable, available in the Wormhole namespace.
 * These classes use a table of member functions to lookup the
 * method to invoke when sending a message from JavaScript.
 *
 * There are two JavaScript to C++ message formats: JSON and string
 * streams. String streams are generally faster, and this is the format
 * used by CustomWebAppMoblet and MessageHandler when invoking custom code.
 *
 * To build this example, you need to have project WormholeExtensions
 * imported into the MoSync IDE (in the same workspace as this project).
 */

#include <Wormhole/WebAppMoblet.h>
#include <Wormhole/MessageProtocol.h>
#include <Wormhole/MessageStream.h>
#include <Wormhole/Libs/JSONMessage.h>
#include <Wormhole/Libs/PhoneGap/PhoneGapMessageHandler.h>
#include <Wormhole/Libs/JSNativeUI/NativeUIMessageHandler.h>
#include <Wormhole/Libs/JSNativeUI/ResourceMessageHandler.h>

#include <CustomWebAppMoblet.h>

#include "MAHeaders.h" // Defines BEEP_WAV

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
		// Show the start page. This will also perform initialization if needed.
		showPage("index.html");

		// TODO: Call from JavaScript last in index.html, or last in wormhole.js ?
		// Have written code in MessageHandler.cpp to call initializePhoneGap().
		// Problem with having it here may be that PhoneGap is not loaded
		// when initializePhoneGap() is called (is this possible?).
		//initializePhoneGap(this);

		// The beep sound is defined in file "Resources/Resources.lst".
		// Note: This used by the PhoneGap beep notification API.
		// Below we add our own beep message, to illustrate how to
		// invoke custom C++ code from JavaScript. Do not confuse these
		// two ways of playing a beep sound, they are completely unrelated.
		setBeepSound(BEEP_WAV);

		// Register functions to handle custom messages sent from JavaScript.
		addMessageFun(
			"Vibrate",
			(FunTable::MessageHandlerFun)&MyMoblet::vibrate);

		// Register functions to handle custom messages sent from JavaScript.
		addMessageFun(
			"Beep",
			(FunTable::MessageHandlerFun)&MyMoblet::beep);
	}

	virtual ~MyMoblet()
	{
		// Add cleanup code as needed.
	}

	void vibrate(Wormhole::MessageStream& message)
	{
		int duration = MAUtil::stringToInteger(message.getNext());
		maVibrate(duration);
	}

	void beep(Wormhole::MessageStream& message)
	{
		// This is how to play a sound using MoSync API.
		maSoundPlay(BEEP_WAV, 0, maGetDataSize(BEEP_WAV));
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
