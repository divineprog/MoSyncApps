# How To Extend JavaScript With Custom C++ Code - Opening Google Maps From A Hybrid App
<!-- C:\md>perl Markdown.pl C:\MoSyncProjects\MoSyncApps\PlatformRequestDemo\Tutorial\PlatformRequestDemo.md > C:\MoSyncProjects\MoSyncApps\PlatformRequestDemo\Tutorial\PlatformRequestDemo.html -->

<!--
<style type="text/css">
p>img {
  width: 550px;
}
</style>
-->

This tutorial shows how write a MoSync Hybrid App that invokes a small piece of C++ code to open web sites in a new window (activity). This is useful if you for example wish to open Google Maps from your application.

The source code for this tutorial is available on [GitHub](https://github.com/divineprog/MoSyncApps/tree/master/PlatformRequestDemo).

## Background

In a Hybrid App, there is a WebView widget that displays HTML content. This WebView is not a fully featured browser in terms of navigation support, cookies etc. When opening a link (e.g. by clicking on it) the target is opened in the same WebView. This can cause problems, for example if you wish to navigate back to the original page displayed in the WebView. Complex web sites may work less well in the embedded WebView widget compared to the full web browser on the device. 

The solution is to use a C++ level syscall called [maPlatformRequest](http://www.mosync.com/files/imports/doxygen/latest/html/maapi_8h.html#a155b042c55d95404035d081c08d40ebf). This opens a URL in a new Activity on Android, in a fully featured web browser. And the back button works to return to your app. To invoke this syscall from JavaScript, you use the JavaScript to C++ bridge available in the MoSync Wormhole API. This API allows you to call any C++ code in your application.

Of course, the situation would be simpler if the functionality of maPlatformRequest would already be available in the Wormhole JavaScript API, but as it goes, it is not in there yet.

Note that if you are using MoSync Reload, calling custom C++ code won't work. You need to build your app with the MoSync SDK for the C++ code to be built and included with your app.

If you have not used the MoSync SDK previously, [read this tutorial to get started quickly](http://www.mosync.com/documentation/manualpages/getting-started-html5-and-javascript).

## Demo application

The app PlatformRequestDemo has a very simple user interface with buttons that open locations on Google Maps.

Screenshots from the app running in the iOS Simulator and on Android Nexus 7:

![PlatformRequestDemo iOS 1](https://raw.github.com/divineprog/MoSyncApps/master/PlatformRequestDemo/Tutorial/PlatformRequestDemo_iOS_1.png)
![PlatformRequestDemo iOS 2](https://raw.github.com/divineprog/MoSyncApps/master/PlatformRequestDemo/Tutorial/PlatformRequestDemo_iOS_2.png)
![PlatformRequestDemo Android 1](https://raw.github.com/divineprog/MoSyncApps/master/PlatformRequestDemo/Tutorial/PlatformRequestDemo_Android_1.png)
![PlatformRequestDemo Android 2](https://raw.github.com/divineprog/MoSyncApps/master/PlatformRequestDemo/Tutorial/PlatformRequestDemo_Android_2.png)

When you try out the app on Android, notice that going back from Google Maps to the app screen requires multiple back button presses (Somehow Google Maps puts in an extra navigation step). On plain web pages, you can go back directly with one press, unless you have navigated any links, in which case you need to press back the corresponding number of times. You can always go to your app again via the Home button. On iOS, you just press the Home key and then go into the app again when you wish to return to it (iOS does not have a back button). 

The source code is available on GitHub. The JavaScript layer of the app is in file [index.html](https://github.com/divineprog/MoSyncApps/blob/master/PlatformRequestDemo/LocalFiles/index.html). The  C++ layer is in file [main.cpp](https://github.com/divineprog/MoSyncApps/blob/master/PlatformRequestDemo/main.cpp).

## What maPlatformRequest does

The syscall maPlatformRequest is in the MoSync C API. It opens a URL in an external application. Typically the URL is a HTTP request, pointing to a web page. But it cal also be a telephone number, used for dialing that number.

Here is an example of opening a web page:

    maPlatformRequest("http://maps.google.com/");

And here is an example of dialing a number:

    maPlatformRequest("tel:046850510300");

Note that if you wish to use this functionality, you must set the "Phone Calls" permission in the MoSync SDK (in the Eclipse IDE). This setting is found by right-click the project and select Properties/MoSync Project/Application Permissions.

## Into the code

Time to dive into the code. We will start with JavaScript and file [index.html](https://github.com/divineprog/MoSyncApps/blob/master/PlatformRequestDemo/LocalFiles/index.html). Here are the functions that compose a Google Maps URL and make the call to C++:

    /**
     * Calls C++ to invoke syscall maPlatformRequest.
     */
    function openMapOnLocation(lat, lon)
    {
        var url = "https://maps.google.com/maps?z=15&t=m&q=loc:"
            + lat + "+" + lon;
        makePlatformRequest(url);
    }
    
    /**
     * Calls C++ to invoke syscall maPlatformRequest.
     */
    function makePlatformRequest(url)
    {
        // Send message with request to C++.
        mosync.bridge.send(["Custom", "PlatformRequest", url]);
    }

You can call the above functions in your code to open an external browser. For example:

    openMapOnLocation(59.339416,18.058755)
    
    makePlatformRequest('http://google,com')

What happens when mosync.bridge.send is called with the first element of the parameter array being "Custom", is that the C++ layer is invoked, and the name given in the second element in the array is looked up in a C++ function table. If the function is found, it is called.

Now let's move to the C++ layer and the file [main.cpp](https://github.com/divineprog/MoSyncApps/blob/master/PlatformRequestDemo/main.cpp). Here is the part of the code that defines the Moblet class of the application (the Moblet is the central object in a MoSync application):

    class MyMoblet : public HybridMoblet
    {
    public:
        /**
         * Constructor.
         */
        MyMoblet()
        {
            // Show the start page.
            showPage("index.html");

            // Register function to handle messages from JavaScript.
            addMessageFun(
                "PlatformRequest",
                (FunTable::MessageHandlerFun)&MyMoblet::handlePlatformRequest);
        }

        /**
         * Function that opens the requested URL.
         */
        void handlePlatformRequest(Wormhole::MessageStream& message)
        {
            // Obtain URL parameter sent from JavaScript.
            const char* url = message.getNext();
            
            // Make the request to open the URL.
            maPlatformRequest(url);
        }
    };

To get a more in-depth explanation of how the JavaScript to C++ bridge works, I suggest you read the tutorials [Extending HTML5 Mobile Apps with C++](http://www.mosync.com/documentation/manualpages/how-communicate-between-javascript-and-c-mosync) and [Using SQL in Hybrid Apps](http://www.mosync.com/documentation/manualpages/using-sql-hybrid-apps). 

## Wrapping up

HTML5 and libraries like PhoneGap brings great power to mobile apps, but there can be times when you encounter functionality that is missing. By making a Hybrid App, which combines JavaScript with native code, the missing functionality can be made available. In the case of the MoSync SDK, functionality not available in JavaScript can be implemented in C/C++, as illustrated by the PlatformRequestDemo application. This is a powerful capability that can significantly improve your apps.
