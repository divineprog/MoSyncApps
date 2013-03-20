/**
 * @file main.cpp
 *
 * This template application shows how to display a
 * splash screen wile loading the HTML app in the WebView.
 */

#include <Wormhole/HybridMoblet.h>

#include "MAHeaders.h"

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
	MyMoblet()
	{
		// Display the splash screen.
		showSplashScreen();

		// Show the start page.
		showPage("index.html");

		// Set the sound used by the PhoneGap beep notification API.
		// BEEP_WAV is defined in file Resources/Resources.lst.
		// Below we add our own beep message, to illustrate how to
		// invoke custom C++ code from JavaScript. Do not confuse these
		// two ways of playing a beep sound.
		setBeepSound(BEEP_WAV);
	}

	// Overridden from HybridMoblet.
	void showPage(const MAUtil::String& url)
	{
		// Extract files system and perform other initialisation.
		initialize();

		// Load the page (WebView is hidden while page loads).
		getWebView()->openURL(url);
	}

	// Overridden from HybridMoblet.
	void openWormhole(MAHandle webViewHandle)
	{
		// Show the WebView.
		showWebView();

		// Continue finalizing the set up of the WebView.
		this->HybridMoblet::openWormhole(webViewHandle);
	}

	void showSplashScreen()
	{
		// Compute coordinates to center image.
		int screenSize = maGetScrSize();
		int screenWidth = EXTENT_X(screenSize);
		int screenHeight = EXTENT_Y(screenSize);

		int imageSize = maGetImageSize(SPLASH_SCREEN);
		int imageWidth = EXTENT_X(imageSize);
		int imageHeight = EXTENT_Y(imageSize);

		int imageX = (screenWidth - imageWidth) / 2;
		int imageY = (screenHeight - imageHeight) / 2;

		// Fill background.
		maSetColor(0x000000);
		maFillRect(0, 0, screenWidth, screenHeight);

		// Draw image centered.
		maDrawImage(SPLASH_SCREEN, imageX, imageY);

		// Display updates.
		maUpdateScreen();
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
