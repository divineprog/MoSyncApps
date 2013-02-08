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
		// Show the start page.
		showPage("index.html");

		// The beep sound is defined in file "Resources/Resources.lst".
		setBeepSound(BEEP_WAV);

		// Turn off zoom.
		//getWebView()->disableZoom();
	}

	virtual ~TwitterMoblet()
	{
		// Add cleanup code as needed.
	}

	/**
	 * This method is called when a key is pressed.
	 */
	/*void keyPressEvent(int keyCode, int nativeCode)
	{
		// This forwards the event to PhoneGapMessageHandler.
		HybridMoblet::keyPressEvent(keyCode, nativeCode);
	}*/
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
