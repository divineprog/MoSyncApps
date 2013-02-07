/**
 * @file main.cpp
 * @author Mikael Kindborg
 *
 * Demo of a simple JavaScript File API, which is
 * implemented in C++.
 */

#include <conprint.h>
#include <Wormhole/HybridMoblet.h>
#include "FileMessageHandler.h"   // Custom File API.
#include "MAHeaders.h"

//#include "ReloadUtil.h"

// Namespaces we want to access.
using namespace MAUtil; // Class Moblet
using namespace NativeUI; // WebView widget.
using namespace Wormhole; // Wormhole library.

/**
 * The application class.
 */
class MyMoblet : public HybridMoblet
{
public:
	MyMoblet()
	{
		//mReloadUtil.setReloadURL("http://192.168.0.157:4042/index.html");
		//mReloadUtil.setMoblet(this);

		// Show the start page. This will also perform initialization if needed.
		showPage("index.html"); // Used for production.
		//reloadPage(); // Used for debugging/dynamic reload.

		// The beep sound is defined in file "Resources/Resources.lst".
		setBeepSound(BEEP_WAV);

		// Turn off zoom.
		getWebView()->disableZoom();

		// Register functions to handle custom messages sent from JavaScript.
		addMessageFun(
			"File",
			(FunTable::MessageHandlerFun)&MyMoblet::handleFileMessage);
	}

	virtual ~MyMoblet()
	{
		// Add cleanup code as needed.
	}

	/**
	 * Simple support for dynamic reloading of the
	 * app uding development.
	 */
	/*void reloadPage()
	{
		char* s = "@@@ Reloading page";
		maWriteLog(s, strlen(s));
		// Enter address of your HTML-sever where you have
		// the application files.
		showPage("http://192.168.0.157:4042/index.html");
	}*/

	/**
	 * This method is called when a key is pressed.
	 */
	/*void keyPressEvent(int keyCode, int nativeCode)
	{
		// This forwards the event to PhoneGapMessageHandler.
		HybridMoblet::keyPressEvent(keyCode, nativeCode);
	}*/

	void handleFileMessage(Wormhole::MessageStream& message)
	{
		mMessageHandler.handleMessage(message);
	}

private:
	/**
	 * Object that handles messages from JavaScript.
	 */
	FileMessageHandler mMessageHandler;

	//ReloadUtil mReloadUtil;
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
