<!-- # Draft: Performance of jQuery Compatible Mobile Frameworks -->
<!-- C:\md>perl Markdown.pl c:\MoSyncProjects\MoSyncApps\JSFrameworkBenchmark\Results\PerformanceArticle.md > output.txt -->

By Mikael Kindborg and Ali Sarrafi

In this article we compare the performance of three JavaScript frameworks, the popular and widely used [jQuery](http://jquery.com/) library, and two jQuery compatible libraries designed for mobile use: [Zepto](http://zeptojs.com/) and [jqMobi](http://www.jqmobi.com/).

## Background

The benefits of [jQuery](http://jquery.com/) include:

* cross-browser compatibility (jQuery abracts away browser differences) 
* high-level syntax (compact notation using the $ function and chained function calls)
* the jQuery API is widely known among web developers

The jQuery library was originally designed for desktop browsers, with cross-browser compatibility as a major goal. Since fragmentation on the desktop is worse than on mobile devices, jQuery comes with some additional weight not really needed for mobile devices. Therefore, new jQuery compatible libraries specifically designed for mobile browsers have surfaced. Two of those are Zepto and jqMobi. While not 100% compatible, they are largely compatible with subsets of jQuery.

[Zepto](http://zeptojs.com/) (also called Zepto.js) aims to be smaller and load faster than jQuery. It supports both modern desktop browsers and mobile browsers.

[jqMobi](http://www.jqmobi.com/) is specifically designed for mobile devices. It is designed to be faster and smaller than jQuery.

The question is, how do the new jQuery flavoured libraries perform? Are they any faster then jQuery? Of specific interest to us is how the libraries perform in the context of a mobile application.

## Benchmark app

[JSFrameworkBenchmark](https://github.com/divineprog/MoSyncApps/tree/master/JSFrameworkBenchmark) is the test app we used to benchmark the libraries. It is a web-based native app, built using [MoSync](http://mosync.com). We ran the app on Android, iOS and WindowsPhone. See below for the results.

A typical scenario when developing a mobile application is to use web-based technologies to code the app, then package the app as a native app. Commonly, files are bundled with the application, rather than downloaded over the Internet. This means that download time is not an issue, but file size could still have an impact on loading time if library files are big.

The app has a Web UI (a WebWidget browser component) that runs on top of a thin layer of C++. The C++ layer contains code that measures startup time and page load time. The WebWidget contains JavaScript code that does DOM manipulation benchmarks.

When started, the application automatically starts running tests. When completed, the results are displayed, as shown in this sample screenshot:

![Screenshot](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/Screenshot.png)

The application uses the same source code for each library. When making a build, for say jQuery, we copy and paste the jquery-1.7.2.min.js file from the Frameworks folder into the LocalFiles folder (and delete any previously used library file), then change the script tag to include the library file. Then we build for the desired target platform. That is all there is to it.

## Test results

We used the following versions of the libraries:

* jQuery 1.7.2 minified
* Zepto 1.0rc1 minified
* jqMobi 1.03 minified

The app tests the following:

* App startup time
* Library loading time
* Performance of DOM manipulation:
    * Create DOM elements (create + style + add)
    * Modify DOM elements (access + modify text)
    * Delete DOM elements (access + remove)

For each type of DOM operation, the app makes 5000 iterations. The number of milliseconds for each 5000 iterations are shown in the diagrams below.

With each library and device, we ran the app once, then killed it and ran it again. The mean values are displayed in the diagrams below. (We also did some extra runs to verify that the result was not off range due to GC or other ongoing processes).

Here follows the results on tested sample devices. Test results are in milliseconds. Smaller values are better.

### Google Nexus One, Android 2.3.6

![Results Nexus One](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/DiagramNexusOne.png)

### HTC Wildfire S, Android 2.3.3

![Results HTC Wildfire S One](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/DiagramHTCWildfireS.png)

### HTC One, Android 4.0.3

![Results HTC One One](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/DiagramHTCOne.png)

### Samsung Galaxy SII, Android 4.0.3

![Results Samsung Galaxy SII One](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/DiagramSamsungGalaxySII.png)

### iPhone 4S, iOS 5.1

![Results iPhone 4S One](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/DiagramIPhone4S.png)

## Discussion

Results show that jqMobi outperforms the other libraries, being much faster than both jQuery and Zepto for DOM operations, and loading slightly faster. jQuery is in general faster for DOM operations than Zepto, but for some operations Zepto performs better. Zepto loads slightly faster than jQuery.

We also did some tests using the non-minified versions of the libraries, and the results show that while loading time is marginally slower, the performance is equal to the minified versions.

Page loading time on iOS is faster when relaunching the app, because of the way iOS caches pages. For example, in our test on iPhone 4S, jqMobi loaded in 843 ms on first app start, and next time loaded in 399 ms. All libraries loaded in roughly this time span. Note that the diagrams show the average of first and second load time.

Depending of the characteristics of your application, the performance of DOM operations may not be very critical. However, if your app contains of a large number of DOM elements, the choice of library can have a big impact of performance.






