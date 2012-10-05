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

class CustomWebAppMoblet;

/**
 * Handler for Wormhole messages of type "Custom".
 *
 * Members are made virtual and public by design, to give
 * maximum flexibility to users of this class. Do not access
 * instance variables directly, unless absolutely needed.
 */
class MessageHandler
{
public:
	MessageHandler();
	virtual ~MessageHandler();

	virtual void initialize(Wormhole::CustomWebAppMoblet* moblet);

	virtual void initializePhoneGap(Wormhole::CustomWebAppMoblet* moblet);

	virtual void setBeepSound(MAHandle beepSound);

	virtual void addMessageFun(
		const char* command,
		Wormhole::FunTable::MessageHandlerFun fun);

	virtual void callMessageFun(
		const char* command,
		Wormhole::MessageStream& stream,
		Wormhole::CustomWebAppMoblet* moblet);

	virtual void keyPressEvent(int keyCode, int nativeCode);

	virtual void handleWebViewMessage(
		NativeUI::WebView* webView,
		MAHandle data,
		Wormhole::CustomWebAppMoblet* moblet);

	virtual void handleMessageStreamJSON(
		NativeUI::WebView* webView,
		MAHandle data);

	virtual void handleMessageStream(
		NativeUI::WebView* webView,
		MAHandle data,
		Wormhole::CustomWebAppMoblet* moblet);

	virtual void handleMoSyncMessage(
		Wormhole::MessageStream& message,
		NativeUI::WebView* webView,
		Wormhole::CustomWebAppMoblet* moblet);

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
	Wormhole::FunTable mFunTable;
};

} // namespace

#endif
