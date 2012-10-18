/*
* @file app.js
* @author Mikael Kindborg
*
* Application code common to all TwitterReader apps.
*/

/**
 * Application layer (non-UI code).
 */
var app = (function()
{
	var app = {};

	/**
	 * Downloads Tweets for a specific user.
	 * @param user Twitter user name.
	 * @param callbackFun Function called when download has
	 * finished. Parameters: callbackFun(tweetArray).
	 */
	app.getTweetsForUser = function(user, callbackFun)
	{
		console.log("@@@@ getTweetsForUser 1");

		// Create the Twitter url.
		var twitterUrl =
			"http://api.twitter.com/1/statuses/" +
			"user_timeline.json?screen_name=" +
			user;

		// Do ajax call to get an array of Tweets.
		$.getJSON(twitterUrl, function(tweets)
		{
			console.log("@@@@ getTweetsForUser 2");
			callbackFun(tweets);
			console.log("@@@@ getTweetsForUser 3");
		});

		console.log("@@@@ getTweetsForUser 4");
	};

	/**
	 * Read the list of favourite users.
	 * @param callbackFun Called with parameters:
	 * callbackFun(rawUserData, filePath)
	 */
	app.readFavouriteUsers = function(callbackFun)
	{
		console.log("@@@ readFavouriteUsers");

		// Get path to local file directory, then
		// read favourite users from that directory.
		mosync.file.getLocalPath(function(path)
		{
			if (!path)
			{
				// Report error.
				callbackFun(null, null);
			}
			else
			{
				var filePath = path + "SavedUsers";
				mosync.file.read(
					filePath,
					function(data)
					{
						callbackFun(data, filePath);
					});
			}
		});
	};

	/**
	 * This function calls MoSync C++ code for accessing files
	 * and saving favourite users.
	 * @param userName Name of Twitter user to save.
	 * @param callbackFun Function called on success or error.
	 * Parameters: callbackFun(userList).
	 */
	app.addFavouriteUser = function(userName, callbackFun)
	{
		console.log("@@@ addFavouriteUser");

		// First read users, then add the new user name.
		app.readFavouriteUsers(function(userData, filePath)
		{
			// Initialise to empty string if null.
			var userList = !userData ? "" : userData;

			// Create a unique string, "!" is not in any
			// Twitter user name.
			var separatedUserName = "!" + userName + "!";

			// Add user only if not in string.
			if (-1 === userList.indexOf(separatedUserName))
			{
				userList += separatedUserName;

				// Write user list.
				mosync.file.write(
					filePath,
					userList,
					function(success)
					{
						if (success)
						{
							callbackFun(userList);
						}
						else
						{
							callbackFun(null);
						}
					});
			}
		});
	};

	/**
	 * Parse a string with a list of user names.
	 * @param userData Format of userData is "!user1!!user2!!...!!userN!"
	 * @return Array with user names.
	 */
	app.parseFavouriteUsers = function(userData)
	{
		// Split string into elements, remove empty strings.
		return userData.split("!").filter(
			function(x) { return x.length > 0 });
	};

	return app;
})();
