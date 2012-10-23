/**
 * @file main.cpp
 *
 * This app demonstrates how to make a hybrid app that uses the
 * MoSync Database C API. The app itself is a small game.
 * More information is given in the tutorial document, which
 * is available in the Tutorial folder (and also published on
 * www.mosync.com).
 *
 * This file contains the C++ support code for the app. The main part
 * if the implementation of the database services for the app. These
 * are invoked from JavScript to store and read data.
 */

#include <conprint.h>
#include <Wormhole/HybridMoblet.h>
#include "MAHeaders.h"

// Namespaces we want to access.
using namespace MAUtil; // Class Moblet
using namespace NativeUI; // WebView widget.
using namespace Wormhole; // Wormhole library.


/**
 * Utility class that simplifies using the Database API.
 */
class DBUtil
{
public:
	/**
	 * Open a database.
	 * @return Handle to database, < 0 on error.
	 */
	static MAHandle openDatabase(const char* fileName)
	{
		// Open the database. It will be created if it does not exist.
		// The database will be created in the local directory of the
		// application.
		MAUtil::String path = DBUtil::getLocalPath();
		path += fileName;
		MAHandle db = maDBOpen(path.c_str());
		return db;
	}

	/**
	 * Get the path to the local file system.
	 * @return Path that ends with a slash.
	 */
	static String getLocalPath()
	{
		int bufferSize = 1024;
		char buffer[bufferSize];

		int size = maGetSystemProperty(
			"mosync.path.local",
			buffer,
			bufferSize);

		// If there was an error, return default root path.
		if (size < 0 || size > bufferSize)
		{
			return "/";
		}

		return buffer;
	}

	/**
	 * Utility function that gets a text field as a string, using the
	 * maDBCursorGetColumnText function.
	 * @param cursor Handle to a cursor object.
	 * @param column Zero-based index to the column to retrieve.
	 * @param path Reference to string that will receive the field data.
	 * @return MA_DB_OK on success, MA_DB_NULL if the field value is NULL,
	 * or MA_DB_ERROR on error.
	 */
	static int getColumnString(MAHandle cursor, int column, String& text)
	{
		int bufferSize = 1024;
		char buffer[bufferSize];

		// Get the text value of the field.
		int size = maDBCursorGetColumnText(cursor, column, buffer, bufferSize);

		// If it went ok, copy text to result string.
		if (size >= 0 && size < bufferSize)
		{
			// The text buffer is not zero terminated, so we do that now.
			buffer[size] = 0;

			// Copy to result string.
			text = buffer;

			return MA_DB_OK;
		}
		else if (MA_DB_NULL == size)
		{
			return MA_DB_NULL;
		}
		else
		{
			return MA_DB_ERROR;
		}
	}

};

/**
 * The application class.
 */
class AppMoblet : public HybridMoblet
{
public:
	/**
	 * Constructor.
	 */
	AppMoblet()
	{
		// Show the start page. This will also perform initialization if needed.
		showPage("index.html");

		// The beep sound is defined in file "Resources/Resources.lst".
		setBeepSound(BEEP_WAV);

		// Turn on zooming of the UI.
		getWebView()->enableZoom();

		// Register functions to handle custom messages sent from JavaScript.
		addMessageFun(
			"GetScore",
			(FunTable::MessageHandlerFun)&AppMoblet::serviceGetScore);
		addMessageFun(
			"SetScore",
			(FunTable::MessageHandlerFun)&AppMoblet::serviceSetScore);

		// Open/initialize the database.
		openDatabase();
	}

	/**
	 * Destructor.
	 */
	virtual ~AppMoblet()
	{
		// Add cleanup code as needed.
		closeDatabase();
	}

	/**
	 * Open the database and create tables and rows.
	 */
	void openDatabase()
	{
		// Open the database.
		mDB = DBUtil::openDatabase("GameDB");

		// Do we have any rows?
		MAHandle cursor = maDBExecSQL(mDB, "SELECT * FROM player");
		if (cursor < 1)
		{
			// No rows exist, initialize the database.
			maDBExecSQL(mDB, "DROP TABLE IF EXISTS player");
			maDBExecSQL(mDB, "CREATE TABLE player (name, score)");
			maDBExecSQL(mDB, "INSERT INTO player VALUES ('Me', 0)");
			maDBExecSQL(mDB, "INSERT INTO player VALUES ('You', 0)");
		}
		else
		{
			// If we got a valid cursor data exists, but we need
			// to release the cursor.
			maDBCursorDestroy(cursor);
		}
	}

	void closeDatabase()
	{
		maDBClose(mDB);
	}

	/**
	 * Get get the score of a named player and return the result
	 * to a JavaScript callback function.
	 * @param message
	 */
	void serviceGetScore(Wormhole::MessageStream& message)
	{
		lprintfln("@@@ serviceGetScore");

		// Get the name of the player.
		const char* name = message.getNext();

		// Query the score for the given name.
		char query[128];
		sprintf(query, "SELECT score FROM player WHERE name='%s'", name);
		MAHandle cursor = maDBExecSQL(mDB, query);
		if ((cursor > 0) && (MA_DB_OK == maDBCursorNext(cursor)))
		{
			// Read the score into a String object.
			String score;
			int result = DBUtil::getColumnString(
				cursor, // Cursor handle.
				0,      // Index of column (we have only one column in the
				        // result, zero is the index of the first column).
				score   // String set to the score value.
				);
			if (MA_DB_OK == result)
			{
				lprintfln("@@@ serviceGetScore result: %s", score.c_str());

				// Return result to JavaScript and exit this function.
				callCallbackWithResult(message, score.c_str(), true);
				maDBCursorDestroy(cursor);
				return;
			}
			maDBCursorDestroy(cursor);
		}

		// Return error to JavaScript.
		callCallbackWithResult(message, "Could not read score", false);
	}

	/**
	 * Set get the score of a named player.
	 * @param message
	 */
	void serviceSetScore(Wormhole::MessageStream& message)
	{
		lprintfln("@@@ serviceSetScore");

		// Get the name of the player.
		const char* name = message.getNext();

		// Get the new score value.
		const char* score = message.getNext();

		// Update the score for the given name.
		char query[128];
		sprintf(query, "UPDATE player SET score=%s WHERE name='%s'", score, name);
		MAHandle result = maDBExecSQL(mDB, query);
		if (MA_DB_OK == result)
		{
			callCallbackWithResult(message, "Updated score", true);
		}
		else
		{
			callCallbackWithResult(message, "Failed to update score", false);
		}
	}

	/**
	 * Calls a JavaScript callback function using the "callbackId"
	 * parameter. The callbackId is supplied automatically when
	 * calling mosync.bridge.send wuth a callback function.
	 * @param message The message stream from which to get the
	 * callback id.
	 * @param result A string that contains the data to be returned
	 * to the JavaScript callback function.
	 * @param success Success value passed back to JavaScript, used
	 * to indicate success or error of a call.
	 */
	void callCallbackWithResult(
		Wormhole::MessageStream& message,
		const String& result,
		bool success)
	{
		// Get the callbackID parameter.
		const char* callbackId = message.getNext();

		// Call JavaScript reply handler.
		String successValue = success ? "true" : "false";
		String script = "mosync.bridge.reply(";
		script += callbackId;
		script += ",'" + result + "'";
		script += "," + successValue + ")";

		lprintfln("@@@ JS: %s", script.c_str());

		message.getWebView()->callJS(script);
	}

	void reloadPage()
	{
		char* s = "@@@ Reloading page";
		maWriteLog(s, strlen(s));
		showPage("http://192.168.0.104:4042/index.html");
		//showPage("js/web-app/index.html");
	}

	/**
	 * This method is called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
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

private:
	/**
	 * Handle to the database object.
	 */
	MAHandle mDB;
};

/**
 * Main function that is called when the program starts.
 * Here an instance of the MyMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain()
{
	(new AppMoblet())->enterEventLoop();
	return 0;
}
