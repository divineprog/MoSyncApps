/**
 * @file main.cpp
 * @author Mikael Kindborg
 *
 * Application that reads Twitter flows. Based on the MoSync
 * example app WebViewTwitter.
 *
 * The entire UI and most of the application logic is implemented
 * in JavaScript.
 *
 * This program illustrates how to send messages between JavaScript
 * and C++. The app uses a simple JavaScript File API, which is
 * implemented in C++.
 */

#include <conprint.h>
#include <Wormhole/HybridMoblet.h>
#include "FileMessageHandler.h"   // Custom File API.
#include "MAHeaders.h"

// Namespaces we want to access.
using namespace MAUtil; // Class Moblet
using namespace NativeUI; // WebView widget.
using namespace Wormhole; // Wormhole library.

/**
 * The application class.
 */
class TwitterMoblet : public HybridMoblet
{
public:
	TwitterMoblet()
	{
		// Show the start page. This will also perform initialization if needed.
		//showPage("index.html"); // Used for production.
		reloadPage() // Used for debugging/dynamic reload.

		// The beep sound is defined in file "Resources/Resources.lst".
		setBeepSound(BEEP_WAV);

		// Turn off zoom.
		getWebView()->disableZoom();

		// Register functions to handle custom messages sent from JavaScript.
		addMessageFun(
			"File",
			(FunTable::MessageHandlerFun)&TwitterMoblet::handleFileMessage);
	}

	virtual ~TwitterMoblet()
	{
		// Add cleanup code as needed.
	}

	/**
	 * Simple support for dynamic reloading of the
	 * app uding development.
	 */
	void reloadPage()
	{
		char* s = "@@@ Reloading page";
		maWriteLog(s, strlen(s));
		// Enter address of your HTML-sever where you have
		// the application files.
		showPage("http://192.168.0.157:4042/index.html");
	}

	/**
	 * This method is called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		lprintfln("@@@ keyPressEvent: %i %i\n", keyCode, nativeCode);

		if (MAK_MENU == keyCode || 0 == keyCode)
		{
			reloadPage();
		}
		else
		{
			// This forwards the event to PhoneGapMessageHandler.
			HybridMoblet::keyPressEvent(keyCode, nativeCode);
		}
	}

	void handleFileMessage(Wormhole::MessageStream& message)
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
	(new TwitterMoblet())->enterEventLoop();
	return 0;
}
