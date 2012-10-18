/*
* @file app.ui.js
* @author Mikael Kindborg
*
* UI code common to all TwitterReader apps.
*/

/**
 * UI code common to all JavaScript frameworks.
 */
app.ui = (function()
{
	var ui = {};

	// Make sure we get the correct event type on every touch system.
	//ui.clickEvent = mosync.isIOS ? 'tap' : 'click';

	ui.showMessage = function(message)
	{
		// Windows Phone does not support alert, show the message
		// in the user name field for now.
		if (mosync.isWindowsPhone)
		{
			ui.setUserNameField(message);
		}
		else
		{
			alert(message);
		}
	};

	/**
	 * Get a DOM element by ID.
	 */
	ui.get = function(id)
	{
		return document.getElementById(id);
	};

	ui.getUserNameField = function()
	{
		return ui.get("userNameField").value;
	};

	ui.setUserNameField = function(name)
	{
		ui.get("userNameField").value = name;
	};

	/**
	 * Displays the Twitter user name in the UI.
	 * @param user The Twitter user name.
	 */
	ui.showTweetListUserName = function(user)
	{
		var heading = ui.get("pageTitle");
		heading.innerHTML = user;
	};

	/**
	 * Displays a loading indicator in the UI.
	 */
	ui.showLoadingIndicator = function()
	{
		var resultsList = ui.get("tweetList");
		resultsList.innerHTML = "<li>Loading...</li>";
	};

	/**
	 * Show the tweets for the user in the input form.
	 */
	ui.showTweets = function()
	{
		console.log("@@@@ showTweets 1");

		// Get the user name from the input field.
		var user = app.ui.getUserNameField();
		console.log("@@@@ showTweets 2");
		if (user.length === 0)
		{
			console.log("@@@@ showTweets 3");
			ui.showMessage("ui.showTweets: Invalid username");
		}
		else
		{
			console.log("@@@@ showTweets 4");
			ui.showTweetsForUser(user);
		}
		console.log("@@@@ showTweets 5");
	};

	/**
	 * Outputs the received tweets to the user-interface.
	 * @param tweets The tweets as an array. Each element
	 * represents one tweet and can be accessed via tweets[i].text
	 */
	ui.showTweetsForUser = function(user)
	{
		console.log("@@@@ showTweetsForUser: "  + user);
		// Go to second screen.
		ui.showPage("tweetList");
		ui.showTweetListUserName(user);
		ui.showLoadingIndicator();

		// Get tweets.
		app.getTweetsForUser(user, function(tweets)
		{
			ui.showTweetList(tweets);
		});
	};

	/**
	 * Outputs the list of tweets to the user-interface.
	 * @param tweets The tweets as an array. Each element
	 * represents one tweet and can be accessed via tweets[i].text
	 */
	ui.showTweetList = function(tweets)
	{
		var results = "";
		if ((!tweets) || (tweets.length === 0) || (tweets.error === "Not found"))
		{
			results = "<li>No tweets found</li>";
		}
		else
		{
			for (var i = 0; i < tweets.length; ++i)
			{
				results += "<li>" + tweets[i].text + "</li>";
			}
		}
		ui.get("tweetList").innerHTML = results;
	};

	/**
	 * Read and display list of favourite users.
	 */
	ui.readAndDisplayFavouriteUsers = function()
	{
		app.readFavouriteUsers(function(userData, path)
		{
			if (userData)
			{
				ui.displayFavouriteUsers(userData);
			}
		});
	};

	/**
	 * Add a user to the favourite list.
	 */
	ui.addFavouriteUser = function()
	{
		console.log("@@@ addFavouriteUser");

		// Get the username from the input field
		var userName = app.ui.getUserNameField();
		if (userName.length === 0)
		{
			ui.showMessage("Invalid username")
			ui.showPage("home");
			return;
		}

		app.addFavouriteUser(userName, function(userList)
		{
			if (!userList)
			{
				app.ui.showMessage("Could not save user");
			}
			else
			{
				ui.displayFavouriteUsers(userList);
				ui.showMessage("User successfully saved");
			}
		});
	};

	/**
	 * Diplay the list of favourite users in the UI.
	 * @param userData Array of user names.
	 */
	ui.displayFavouriteUsers = function(userData)
	{
		
		var users = app.parseFavouriteUsers(userData);
		var list = "";
		for (var i = 0; i < users.length; ++i)
		{
			list += "<li><a onclick='app.ui.showTweetsForUser(\"" +
				users[i] +
				"\")'>" +
				users[i] +
				"</a></li>";
		}
		ui.get("favouriteList").innerHTML = list;
	};

	/**
	 * Keydown event listener. Gets called when the user presses
	 * a button on his/her keyboard in the input field.
	 * @param keynum The pressed key (as an ASCII value).
	 */
	ui.keydown = function(keynum)
	{
		console.log("@@@@ keydown: " + keynum);
		if (keynum == 13)
		{
			console.log("@@@@ keydown 2: " + keynum);
			//setTimeout(function () {
				console.log("@@@@ calling submitButton");
				//("#submitButton").trigger(clickEvent);
				ui.showTweets();
			//}, 1);
		}
		console.log("@@@@ keydown 3: " + keynum);
	};

	/**
	 * Set up event bindings and initialize the UI.
	 */
	ui.initialize = function()
	{
		// Handle the back key event (on Android).
		document.addEventListener(
			"backbutton",
			function()
			{
				ui.goBack();
			},
			true);

		ui.initializeFramework();

		ui.readAndDisplayFavouriteUsers();
	};

	return ui;
})();

// Call app.ui.initialize when document has loaded.
document.addEventListener(
	"DOMContentLoaded",
	app.ui.initialize,
	false);
