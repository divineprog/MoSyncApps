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
 * @file CustomWebAppMoblet.cpp
 *
 * @author Mikael Kindborg
 */

#include "CustomWebAppMoblet.h"

using namespace MAUtil;

namespace Wormhole
{

CustomWebAppMoblet::CustomWebAppMoblet()
{
	mInitialized = false;
}

CustomWebAppMoblet::~CustomWebAppMoblet()
{
}

void CustomWebAppMoblet::showPage(const MAUtil::String& url)
{
	initialize();
	WebAppMoblet::showPage("index.html");
}

void CustomWebAppMoblet::initialize()
{
	if (mInitialized) { return; }

	mInitialized = true;

	// Extract files in LocalFiles folder to the device.
	extractFileSystem();

	// Enable message sending from JavaScript to C++.
	enableWebViewMessages();

	// Hide the main WebView (we will use NativeUI to display
	// widgets and use this hidden WebView to handle application
	// logic written in JavaScript).
	getWebView()->setVisible(true);

	// The beep sound is defined in file "Resources/Resources.lst".
	mMessageHandler.initialize(this);
}

void CustomWebAppMoblet::initializePhoneGap(
	Wormhole::CustomWebAppMoblet* moblet)
{
	mMessageHandler.initializePhoneGap(moblet);
}

void CustomWebAppMoblet::setBeepSound(MAHandle beepSoundResource)
{
	mMessageHandler.setBeepSound(beepSoundResource);
}

void CustomWebAppMoblet::addMessageFun(
	const char* command,
	FunTable::MessageHandlerFun fun)
{
	mMessageHandler.addMessageFun(command, fun);
}

void CustomWebAppMoblet::handleWebViewMessage(
	NativeUI::WebView* webView,
	MAHandle data)
{
	mMessageHandler.handleWebViewMessage(webView, data, this);
}

/**
 * This method is called when a key is pressed.
 * Eventually forwards the event to PhoneGapMessageHandler.
 */
void CustomWebAppMoblet::keyPressEvent(int keyCode, int nativeCode)
{
	mMessageHandler.keyPressEvent(keyCode, nativeCode);
}

} // namespace
