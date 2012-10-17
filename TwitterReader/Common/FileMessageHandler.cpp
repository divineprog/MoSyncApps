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
 * @file FileMessageHandler.cpp
 * @author Mikael Kindborg
 *
 * Implementation of system calls made from JavaScript.
 */

#include <Wormhole/FileUtil.h>
#include "FileMessageHandler.h"

using namespace MAUtil;
using namespace Wormhole;

/**
 * Constructor.
 */
FileMessageHandler::FileMessageHandler()
{
}

/**
 * Destructor.
 */
FileMessageHandler::~FileMessageHandler()
{
	// Nothing needs to be explicitly destroyed.
}

#include <conprint.h>

/**
 * Implementation of standard API exposed to JavaScript.
 * @return true if message was handled, false if not.
 */
bool FileMessageHandler::handleMessage(Wormhole::MessageStream& message)
{
	const char* operation = message.getNext();
	//int webViewHandle = MAUtil::stringToInteger(message.getNext());

	if (0 == strcmp(operation, "mosync.file.getLocalPath"))
	{
		handleFileGetLocalPath(message);
	}
	else if (0 == strcmp(operation, "mosync.file.read"))
	{
		handleFileRead(message);
	}
	else if (0 == strcmp(operation, "mosync.file.write"))
	{
		handleFileWrite(message);
	}
	else if (0 == strcmp(operation, "mosync.log"))
	{
		handleLog(message);
	}
	else
	{
		// Message was not handled.
		return false;
	}

	// Message was handled.
	return true;
}

/**
 * Handle the getLocalPath message.
 */
void FileMessageHandler::handleFileGetLocalPath(
	Wormhole::MessageStream& message)
{
	FileUtil fileUtil;
	// Note: fileUtil.getLocalPath() does not work correctly on Windows Phone
	// because of a bug in maGetSystemProperty for "mosync.path.local".
	// Here is a fix for that.
	String path = fileUtil.getLocalPath();
	if (0 == path.length())
	{
		// This is a hack to make the program work on Windows Phone.
		replyString(message, "/");

		// Original code was:
		// replyNull(message);
	}
	else
	{
		replyString(message, path);
	}
}

/**
 * Handle the file read message.
 */
void FileMessageHandler::handleFileRead(Wormhole::MessageStream& message)
{
	FileUtil fileUtil;
	const char* filePath = message.getNext();
	String inText;
	bool success = fileUtil.readTextFromFile(filePath, inText);
	if (success)
	{
		replyString(message, inText);
	}
	else
	{
		replyNull(message);
	}
}

/**
 * Handle the file write message.
 */
void FileMessageHandler::handleFileWrite(Wormhole::MessageStream& message)
{
	FileUtil fileUtil;
	const char* filePath = message.getNext();
	const char* data = message.getNext();
	bool success = fileUtil.writeTextToFile(filePath, data);
	if (success)
	{
		replyBoolean(message, true);
	}
	else
	{
		replyString(message, false);
	}
}

/**
 * Handle the log message.
 */
void FileMessageHandler::handleLog(Wormhole::MessageStream& message)
{
	const char* s = message.getNext();
	maWriteLog(s, strlen(s));
}

/**
 * Calls a JavaScript callback function using the "callbackId"
 * parameter. The callbackId is supplied automatically when
 * using the mosync.bridge.sendJSON function.
 * @param result A string that contains the data to be returned
 * to the JavaScript callback function.
 * @return true on success, false on error.
 */
void FileMessageHandler::replyString(
	Wormhole::MessageStream& message,
	const String& result)
{
	// Get the callbackID parameter.
	const char* callbackId = message.getNext();

	// Call JavaScript reply handler.
	String script = "mosync.bridge.reply(";
	script += callbackId;
	script += ", '" + result + "')";

	message.getWebView()->callJS(script);
}

/**
 * Calls a JavaScript callback function using the "callbackId"
 * parameter. The callbackId is supplied automatically when
 * using the mosync.bridge.sendJSON function.
 * @param result A boolean to be returned
 * to the JavaScript callback function.
 * @return true on success, false on error.
 */
void FileMessageHandler::replyBoolean(Wormhole::MessageStream& message, bool result)
{
	// Get the callbackID parameter.
	const char* callbackId = message.getNext();

	// Set truth value.
	String truth = result ? "true" : "false";

	// Call JavaScript reply handler.
	String script = "mosync.bridge.reply(";
	script += callbackId;
	script += "," + truth + ")";

	message.getWebView()->callJS(script);
}

/**
 * Calls a JavaScript callback function using the "callbackId"
 * parameter. Returns null to the callback function.
 * The callbackId is supplied automatically when
 * using the mosync.bridge.sendJSON function.
 * @return true on success, false on error.
 */
void FileMessageHandler::replyNull(Wormhole::MessageStream& message)
{
	// Get the callbackID parameter.
	const char* callbackId = message.getNext();

	// Call JavaScript reply handler.
	String script = "mosync.bridge.reply(";
	script += callbackId;
	script += ", null)";

	message.getWebView()->callJS(script);
}
