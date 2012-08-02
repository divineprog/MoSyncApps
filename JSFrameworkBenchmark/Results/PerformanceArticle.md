# Draft: Performance of jQuery Compatible Mobile Frameworks

In this article we compare the performance of three JavaScript frameworks, the popular and widely used [jQuery](http://jquery.com/) library, and two jQuery compatible libraries designed for mobile use: [Zepto](http://zeptojs.com/) and [jqMobi](http://www.jqmobi.com/).

## Background

The benefits of [jQuery](http://jquery.com/) include:
* cross-browser compatibility (jQuery abracts away browser differences) 
* high-level syntax (compact notiation using the $ function and chained function calls)
* due to its popularity, the jQuery API is widely known among web developers

jQuery has established a standard for JavaScript frameworks, but it was originally designed for desktop browsers. Since fragmentation on the desktop is worse than on mobile devices, jQuery comes with some additional weight not really needed for mobile devices. Therefore, new jQuery compatible libraries specifically designed for mobile browsers have surfaced. Two of those are Zepto and jqMobi. While not 100% compatible, they are largely compatible with subsets of jQuery.

[Zepto](http://zeptojs.com/) (also called Zepto.js) aims to be smaller and load faster than jQuery. It supports both modern desktop browsers and mobile browsers.

[jqMobi](http://www.jqmobi.com/) is specifically designed for mobile devices. It is designed to be faster and smaller than jQuery.

## Performance Test

The question is, are the new jQuery flavoured libraries faster? How do they perform in a mobile web-based application?

To answer these questions, we made a test app that measures: 
* App startup time
* Library loading time
* Performance of DOM manupulation:
    * Create DOM elements
    * Modify DOM elements
    * Delete DOM elements

## Test App Architecture

The test app [JSFrameworkBenchmark](https://github.com/divineprog/MoSyncApps/tree/master/JSFrameworkBenchmark) is a web-based native app. It runs as a stand-alone native app, and is built with using [MoSync](http://mosync.com). The app has been tested on Android, iOS, and WindowsPhone.

The app has a Web UI (a WebWidget browser component) that runs on top of a thin layer of C++. The C++ layer contains code that measures startup time and page load time. The WebWidget contains JavaScript code that does DOM manipulation benchmarks.

When started, the application automatically starts running tests. When completed, the results are displayed, as shown in this sample screenshot:

![Screenshot](/Screenshots/Screenshot.png)

The application uses the same source code for each library. When making a build, for say jQuery, we copy and paste the jquery-1.7.2.min.js file from the Frameworks folder into the LocalFiles folder (and delete any previouly used library file), then change the script tag to include the library file. Then we build for the desired target platform. That is all there is to it.

## Startup/Load Time

The total startup time has been split into 

## DOM Manipulation Tests



We also did some extra runs to verify that the result was not off range due to GC or other hang ups.
