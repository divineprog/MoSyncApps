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
 * @file MessageHandler.h
 * @author Mikael Kindborg
 *
 * Class that abstracts away some of the details of processing
 * messages from Wormhole (JavaScript).
 */

#ifndef WORMHOLE_MESSAGE_HANDLER_H_
#define WORMHOLE_MESSAGE_HANDLER_H_

#include <NativeUI/WebView.h>
#include <Wormhole/WebAppMoblet.h>
#include <Wormhole/MessageProtocol.h>
#include <Wormhole/MessageStream.h>
#include <Wormhole/Libs/JSONMessage.h>
#include <Wormhole/Libs/PhoneGap/PhoneGapMessageHandler.h>
#include <Wormhole/Libs/JSNativeUI/NativeUIMessageHandler.h>
#include <Wormhole/Libs/JSNativeUI/ResourceMessageHandler.h>
#include <Wormhole/Encoder.h>
#include <MAUtil/String.h>
#include <MAUtil/HashMap.h>

#include "FunTable.h"

namespace Wormhole
{
/**
 * Handler for Wormhole messages of type "Custom".
 */
class MessageHandler
{
public:
	MessageHandler();
	virtual ~MessageHandler();

	void init(
		MAHandle beepSound,
		Wormhole::WebAppMoblet* moblet);

	void addMessageFun(
		const char* command,
		Wormhole::FunTable<Wormhole::WebAppMoblet>::MessageHandlerFun fun);

	void callMessageFun(
		const char* command,
		Wormhole::MessageStream& stream,
		Wormhole::WebAppMoblet* moblet);

	void keyPressEvent(int keyCode, int nativeCode);

	void handleWebViewMessage(
		NativeUI::WebView* webView,
		MAHandle data,
		Wormhole::WebAppMoblet* moblet);

	void handleMessageStreamJSON(
		NativeUI::WebView* webView,
		MAHandle data);

	void handleMessageStream(
		NativeUI::WebView* webView,
		MAHandle data,
		Wormhole::WebAppMoblet* moblet);

private:
	/**
	 * Handler for PhoneGap messages.
	 */
	Wormhole::PhoneGapMessageHandler* mPhoneGapMessageHandler;

	/**
	 * Handler for NativeUI messages.
	 */
	Wormhole::NativeUIMessageHandler* mNativeUIMessageHandler;

	/**
	 * Handler for resource messages used for NativeUI.
	 */
	Wormhole::ResourceMessageHandler* mResourceMessageHandler;

	/**
	 * Dictionary with pointers to message handler functions.
	 * command -> fun
	 */
	Wormhole::FunTable<Wormhole::WebAppMoblet> mFunTable;
};

} // namespace

#endif
