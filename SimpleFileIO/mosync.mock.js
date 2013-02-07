/*
* @file mosync.mock.js
* @author Mikael Kindborg
*
* Mockup of some functions in the mosync object.
* Useful for testing in a web browser.
*/
if (!window.mosync)
{
	mosync = {};

	mosync.file = {};

	mosync.file.write = function(path, data, callbackFun)
	{
	  callbackFun(true);
	};

	mosync.file.read = function(path, callbackFun)
	{
	  //callbackFun("!mosync!!divineprog!");
	  callbackFun(null);
	};

	mosync.file.getLocalPath = function(callbackFun)
	{
	  callbackFun("/");
	};

	mosync.isWindowsPhone = false;
	mosync.isIOS = false;
}
