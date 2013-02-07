/*
* @file app.js
* @author Mikael Kindborg
*
* Code for the application layer. The app is divided into
* separate objects for the application logic (the app object)
* and the user interface (the app.ui object).
*/

/**
 * Application layer (non-UI code).
 */
var app = (function()
{
	var app = {};

	/**
	 * Helper function to determine if Local Storage
	 * is available.
	 */
	function hasLocalStorage()
	{
		return typeof(Storage) !== "undefined";
	}
  
	/**
	 * Downloads Tweets for a specific user.
	 * @param user Twitter user name.
	 * @param callbackFun Function called when download has
	 * finished. Parameters: callbackFun(tweetArray).
	 */
	app.getTweetsForUser = function(user, callbackFun)
	{
		// Create the Twitter url.
		var twitterUrl =
			"http://api.twitter.com/1/statuses/" +
			"user_timeline.json?screen_name=" +
			user;

		// Do ajax call to get an array of Tweets.
		$.getJSON(twitterUrl, function(tweets)
		{
			callbackFun(tweets);
		}).error(function(jqXHR, textStatus, errorThrown)
		{
			//alert("getJSON error: " + textStatus);
			callbackFun(null);
		});
	};

	/**
	 * Read the list of favourite users.
	 * @return String with list of users.
	 */
	app.readFavouriteUsers = function()
	{
		if (hasLocalStorage())
		{
			return localStorage.favouriteUsers;
		}
		else
		{
			return null;
		}
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
		// First read users, then add the new user name.
		var userData = app.readFavouriteUsers();

		// Initialise to empty string if null.
		var userList = !userData ? "" : userData;

		// Create a unique string, "!" is not in any
		// Twitter user name.
		var separatedUserName = "!" + userName + "!";

		// Add user only if not in string.
		if (-1 === userList.indexOf(separatedUserName))
		{
			userList += separatedUserName;

			// Save user list.
			if (hasLocalStorage())
			{
				localStorage.favouriteUsers = userList;
				callbackFun(userList);
			}
			else
			{
				callbackFun(null);
			}
		}
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
