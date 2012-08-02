/*
Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file main.cpp
 * @author Mikael Kindborg
 *
 * This is a benmark app for testing performance of JavaScript frameworks.
 */

#include <Wormhole/WebAppMoblet.h>
#include <Wormhole/MessageStream.h>
#include "MyMessageHandler.h"
#include "MAHeaders.h"

/**
 * Time stamp used to measure startup time.
 */
static int gStartUpTimeStamp;

/**
 * Time stamp used to measure page load time.
 */
static int gPageLoadTimeStamp;

/**
 * Startup time.
 */
static int gStartUpTime = 0;

/**
 * Page load time.
 */
static int gPageLoadTime = 0;

/**
 * Page ready time.
 */
static int gPageReadyTime = 0;

/**
 * The application class.
 */
class MyMoblet : public Wormhole::WebAppMoblet
{
public:
	MyMoblet() :
		mMyMessageHandler(getWebView())
	{
		// Extract files in LocalFiles folder to the device.
		extractFileSystem();

		// Enable message sending from JavaScript to C++.
		enableWebViewMessages();

		// Show the WebView.
		getWebView()->setVisible(true);

		// Initial startup time.
		gStartUpTime = maGetMilliSecondCount() - gStartUpTimeStamp;

		// Timing of page load starts here.
		gPageLoadTimeStamp = maGetMilliSecondCount();

		// Load page in the "LocalFiles" folder into the hidden WebView.
		//getWebView()->openURL("index.html");
		showPage("index.html");

		// The beep sound is defined in file "Resources/Resources.lst".
		mMyMessageHandler.init(BEEP_WAV, this);

		// Register functions to handle custom messages sent from JavaScript.
		mMyMessageHandler.addMessageFun(
			"PageLoaded",
			(MyMessageHandlerFun)&MyMoblet::pageLoaded);

		// Register functions to handle custom messages sent from JavaScript.
		mMyMessageHandler.addMessageFun(
			"PageReady",
			(MyMessageHandlerFun)&MyMoblet::pageReady);
	}

	virtual ~MyMoblet()
	{
	}

	/**
	 * This method is called when a key is pressed.
	 * Forwards the event to PhoneGapMessageHandler.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		// Forward to MyMessageHandler.
		mMyMessageHandler.keyPressEvent(keyCode, nativeCode);
	}

	/**
	 * Called from JavaScript when the page is loaded.
	 * Here we measure the time it took to load the page.
	 */
	void pageLoaded(Wormhole::MessageStream& stream)
	{
		gPageLoadTime = maGetMilliSecondCount() - gPageLoadTimeStamp;
		sendStartupResults();
	}

	/**
	 * Called from JavaScript when the page is loaded.
	 * Here we measure the time it took to load the page.
	 */
	void pageReady(Wormhole::MessageStream& stream)
	{
		gPageReadyTime = maGetMilliSecondCount() - gPageLoadTimeStamp;
		sendStartupResults();
	}

	/**
	 * We don't know which will come first, PageLoad or PageReady,
	 * here we guard against this and send results to JavaScript
	 * only when we have both values.
	 */
	void sendStartupResults()
	{
		if (gPageLoadTime != 0 && gPageReadyTime != 0)
		{
			char script[2048];
			sprintf(script,
				"app.loadingCompleted({"
				"startUpTime:%i,"
				"pageLoadTime:%i,"
				"pageReadyTime:%i})",
				gStartUpTime,
				gPageLoadTime,
				gPageReadyTime);
			lprintfln("@@@ script: %s", script);
			callJS(getWebView()->getWidgetHandle(), script);
		}
	}

	/**
	 * Evaluate JavaScript code in the given WebView.
	 */
	void callJS(MAWidgetHandle webViewHandle, const MAUtil::String& script)
	{
		// Call the JavaScript code on the WebView.
		MAUtil::String url = "javascript:" + script;
		maWidgetSetProperty(
			webViewHandle,
			MAW_WEB_VIEW_URL,
			url.c_str());
	}

	/**
	 * This method handles messages sent from the WebView.
	 *
	 * Note that the data object will be valid only during
	 * the life-time of the call of this method, then it
	 * will be deallocated.
	 *
	 * @param webView The WebView that sent the message.
	 * @param urlData Data object that holds message content.
	 */
	void handleWebViewMessage(NativeUI::WebView* webView, MAHandle data)
	{
		// Uncomment to print message data for debugging.
		// You need to build the project in debug mode for
		// the log output to be displayed.
		//printMessage(data);

		mMyMessageHandler.handleWebViewMessage(webView, data, this);
	}

	/**
	 * For debugging.
	 */
	void printMessage(MAHandle dataHandle)
	{
		// Get length of the data, it is not zero terminated.
		int dataSize = maGetDataSize(dataHandle);

		// Allocate buffer for string data.
		char* stringData = (char*) malloc(dataSize + 1);

		// Get the data.
		maReadData(dataHandle, stringData, 0, dataSize);

		// Zero terminate.
		stringData[dataSize] = 0;

		// Print unparsed message data.
		maWriteLog("@@@ MOSYNC Message:", 19);
		maWriteLog(stringData, dataSize);

		free(stringData);
	}

private:
	MyMessageHandler mMyMessageHandler;
};

/**
 * Main function that is called when the program starts.
 * Here an instance of the MyMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain()
{
	gStartUpTimeStamp = maGetMilliSecondCount();

	MAUtil::Moblet::run(new MyMoblet());

	return 0;
}
