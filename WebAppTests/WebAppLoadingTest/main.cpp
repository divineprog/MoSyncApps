/**
 * @file main.cpp
 *
 * Test app that measures how long it takes to
 * load a Wormhole Web App. Results are displayed
 * in the WebView when the app is loaded.
 */

#include <Wormhole/HybridMoblet.h>

#include "MAHeaders.h" // Defines BEEP_WAV

// Namespaces we want to access.
using namespace NativeUI; // WebView widget.
using namespace Wormhole; // Wormhole library.

int gTimeStart;
int gTimeWebViewVisible;
int gTimePageLoaded;

/**
 * The application class.
 */
class MyMoblet : public HybridMoblet
{
public:
	MyMoblet()
	{
		// Show the start page.
		showPage("index.html");

		// Set the sound used by the PhoneGap beep notification API.
		// BEEP_WAV is defined in file Resources/Resources.lst.
		setBeepSound(BEEP_WAV);

		addMessageFun(
			"PageLoaded",
			(FunTable::MessageHandlerFun)&MyMoblet::pageLoaded);
	}

	// Overridden from HybridMoblet.
	void showPage(const MAUtil::String& url)
	{
		// Extract files system and perform other initialisation.
		initialize();

		// Make the main WebView visible.
		getWebView()->setVisible(true);

		// Show the WebView screen.
		showWebView();

		gTimeWebViewVisible = maGetMilliSecondCount();

		// Open the page.
		getWebView()->openURL(url);
	}

	// Called from JavaScript when page has loaded.
	void pageLoaded(Wormhole::MessageStream& message)
	{
		gTimePageLoaded = maGetMilliSecondCount();

		// Display result as a new HTML page.
		char scriptBuf[512];
		sprintf(scriptBuf,
			"document.body.innerHTML="
			"'<h1>Loading Test Result</h1>"
			"<p>WebView visible: %i ms</p>"
			"<p>Page loaded: %i ms</p>'",
			gTimeWebViewVisible - gTimeStart,
			gTimePageLoaded - gTimeStart);
		getWebView()->callJS(scriptBuf);
	}
};

/**
 * Main function that is called when the program starts.
 * Here an instance of the MyMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain()
{
	gTimeStart = maGetMilliSecondCount();
	printf("App Starts\n");

	(new MyMoblet())->enterEventLoop();
	return 0;
}
