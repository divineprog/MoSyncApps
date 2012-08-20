<!-- # Using Web SQL in MoSync Apps  -->
<!-- C:\md>perl Markdown.pl C:\MoSyncProjects\MoSyncApps\WebSQLDemo\WebSQLTutorial.md > output.txt -->

<!--
<style type="text/css">
p>img {
  width: 550px;
}
</style>
-->

In this tutorial we will take a look at the Web SQL Databse API, a standard way of accessing SQL databases from JavaScript. We introduce the API and show how to use it in a MoSync application. In addition, we discuss other available options for accessing databases from a JavaScript-based MoSync app.

The source code for this tutorial is available on [GitHub](https://github.com/divineprog/MoSyncApps/tree/master/WebSQLDemo).

## Background

[Web SQL Database](http://www.w3.org/TR/webdatabase/) is a database API proposed by the W3C Working Group. However, the standard is not actively maintained at present, because of lack of independent implementations (multiple implementations are required for W3C to proceed with the standards process). 

Currently, Web SQL is implemented in WebKit, which means that it is available on iOS and Android (and in Chrome on the destktop).

There exists competing standards for data storage in browsers, as outlined in the article [HTML5 Storage Wars](http://csimms.botonomy.com/2011/05/html5-storage-wars-localstorage-vs-indexeddb-vs-web-sql.html). In the favour of Web SQL is that it is considered reliable and mature, and it is based on SQLite, a proven database engine that has become popular on mobile devices.

## MoSync support for Web SQL

## A tour of the Web SQL API

Use the global function openDatabase to open a [Database](http://www.mosync.com/files/imports/doxygen/latest/html5/database.md.html#Database):

    var db = openDatabase("MyDB", "1", "MyDB", 65536);

Parameters are:

* database name
* database version
* database display name
* database size

Use the transaction function in the database object to excute queries, and execute actual queries using the executeSql function in the [SQLTransaction](http://www.mosync.com/files/imports/doxygen/latest/html5/sqltransaction.md.html#SQLTransaction) object:

    db.transaction(
        function(tx)
        {
            tx.executeSql(
                "INSERT INTO pet VALUES (?, ?, ?)", 
                ["Charmy", 7, 0.6],
                function(tx, result) {
                    console.log("Query Success") },
                function(tx, error) {
                    console.log("Query Error: " + error.message) }
            );
        },
        function(error) {
            // Note that this error function does not get 
            // called if the above query should fail, since
            // we supply an error function to executeSql.
            console.log("Transaction Error: " + error.message) },
        function() {
            console.log("Transaction Success") }
    );

The executeSql function takes up to four parameters:

* query string (mandatory)
* query parameter array (optional)
* query success function (optional)
* query error function (optional)

One nice thing with the Web SQL API is that it encourages the use of query parameters. It is very easy to supply parameters in the query parameter array.

The transaction function takes up to three parameters:

* transaction function, execute queries in this function (mandatory)
* transaction error function (optional)
* transaction success function (optional)

Note the order of the success and error functions, which is the reverse for the transcation function, compared to the executeSql function. Errors are reported using the [SQLError](http://www.mosync.com/files/imports/doxygen/latest/html5/sqlerror.md.html#SQLError) object.

The transaction success and transaction error functions are useful for handling of the entire transaction.

Note that the transaction error function will be called only if you do not supply a query error function to executeSql. If you provide a query error function that will be called, and the transcation success function will be called, rather than the transaction error function. Thus, in the above example, the transaction error function will not be called if the query fails.

Also note that queries are executed asynchronously. When the transaction success function is called, you should be guaranteeded that all queries have been executed.

When doing a search query, the result is obtained as a set of rows, which involves using the [SQLResultSet](http://www.mosync.com/files/imports/doxygen/latest/html5/sqlresultset.md.html#SQLResultSet) and the [SQLResultSetList](http://www.mosync.com/files/imports/doxygen/latest/html5/sqlresultsetlist.md.html#SQLResultSetList) objects:

    tx.executeSql("SELECT * FROM pet", [],
        function(tx, result)
        {
            console.log("All rows:");
            for (var i = 0; i < result.rows.length; ++i)
            {
                var row = result.rows.item(i);
                console.log("  " + row.name + " " + row.age + " " + row.curiosity);
            }
        });

## Basic examples






SQLResultSet
SQLResultSetList
SQLError

of three calls:

* 

## High-level wrapper



## Demo application

The demo app is a simple turn-based game of luck, much like playing by tossing a dice. The "dice" in the app is a "wheel" with numbers 1 tho 50. Whoever gets the highest number wins the current round, and the total score gets updated. 

You challenge the app and can affect the outcome by taking a low risk or a high risk. With high risk, you can double your score, but the app gets to roll the wheel twice.

This is a screenshoot of the app:

![Web SQL Demo Screenshot](https://raw.github.com/divineprog/MoSyncApps/master/WebSQLDemo/Tutorial/WebSQLDemo.png)

The source code is available on [GitHub](https://github.com/divineprog/MoSyncApps/tree/master/WebSQLDemo).


















The benefits of [jQuery](http://jquery.com/) include:

* cross-browser compatibility (jQuery abstracts away browser differences) 
* high-level syntax (compact notation using the $ function and chained function calls)
* the jQuery API is widely known among web developers

The jQuery library was originally designed for desktop browsers, with cross-browser compatibility as a major goal. Since fragmentation on the desktop is worse than on mobile devices, jQuery comes with some additional weight not really needed for mobile devices. Therefore, new jQuery compatible libraries specifically designed for mobile browsers have surfaced. Two of those are Zepto and jqMobi. While not 100% compatible, they are largely compatible with subsets of jQuery.

[Zepto](http://zeptojs.com/) (also called Zepto.js) aims to be smaller and load faster than jQuery. It supports both modern desktop browsers and mobile browsers (with the exception of Internet Explorer).

[jqMobi](http://www.jqmobi.com/) is specifically designed for mobile devices. It is designed to be faster and smaller than jQuery.

The question is, how do the new jQuery flavoured libraries perform? Are they any faster than jQuery? Of specific interest to us is how the libraries perform in the context of a mobile application.

## Benchmark app

[JSFrameworkBenchmark](https://github.com/divineprog/MoSyncApps/tree/master/JSFrameworkBenchmark) is the test app we used to benchmark the libraries. This is a web-based native app, built using the cross-platform development tool [MoSync](http://mosync.com). The app runs without any modifications on Android, iOS and WindowsPhone, with the exception of Zepto, which is not designed to work with Windows Phone/Internet Explorer.

A typical scenario when developing a mobile application is to use web-based technologies to code the app, then package the app as a native application. Commonly, files are bundled with the application, rather than downloaded over the Internet. This means that download time is not an issue, but file size could still have an impact on loading time if library files are big.

The app has a Web UI, with a WebWidget component that runs on top of a thin layer of C++. The C++ layer contains code that measures startup time and page load time. The WebWidget contains JavaScript code that does DOM manipulation benchmarks. Check out GitHub for the [source code](https://github.com/divineprog/MoSyncApps/tree/master/JSFrameworkBenchmark).

When started, the application automatically starts running tests. When completed, the results are displayed on the screen.

The application uses the same source code for each library. We did not need to make any modifications to the application source code when switching between libraries. Library compatibility was 100% for our test app. When making a build, for say jQuery, we copy and paste the jquery-1.7.2.min.js file from the Frameworks folder into the [LocalFiles folder](https://github.com/divineprog/MoSyncApps/tree/master/JSFrameworkBenchmark/LocalFiles) (and delete any previously used library file), then change the script tag in [index.html](https://github.com/divineprog/MoSyncApps/blob/master/JSFrameworkBenchmark/LocalFiles/index.html) to include the library file. Then we build for the desired target platform. That is all there is to it.

## Test results

The platforms tested include:

* Android (various devices and versions)
* iOS (iPhone, iPad)
* Windows Phone 7.5

We used the following versions of the libraries:

* jQuery 1.7.2 minified
* Zepto 1.0rc1 minified
* jqMobi 1.03 minified

The benchmarking app tests the following:

* Page loading time
* Performance of DOM manipulation:
    * Create DOM elements (create + style + add)
    * Modify DOM elements (access + modify text)
    * Delete DOM elements (access + remove)

For each type of DOM operation, the app makes 5000 iterations. The number of milliseconds for each 5000 iterations are shown in the diagrams below.

With each library and device, we ran the app once, then killed it and ran it again. The mean values are displayed in the diagrams below. (We also did some extra runs to verify that the result was not off range due to GC or other ongoing processes).

This photo shows some of the devices tested running the benchmarking application:

![Some devices tested](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/SomeDevicesTested.jpg)

Here follows the results for each device tested. Test results are in milliseconds. Smaller values are better.

### Google Nexus One, Android 2.3.6

![Results Nexus One](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/DiagramNexusOne.png)

### HTC Wildfire S, Android 2.3.3

![Results HTC Wildfire S](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/DiagramHTCWildfireS.png)

### HTC One, Android 4.0.3

![Results HTC One](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/DiagramHTCOne.png)

### Samsung Galaxy SII, Android 4.0.3

![Results Samsung Galaxy SII](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/DiagramSamsungGalaxySII.png)

### iPhone 4S, iOS 5.1

![Results iPhone 4S](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/DiagramIPhone4S.png)

### iPad 3, iOS 5.1

![Results iPad 3](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/DiagramIPad3.png)

### iPad 2, iOS 4.3.5

![Results iPad 2](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/DiagramIPad2.png)

### HTC Titan x310e, Windows Phone 7.5

![Results HTC Titan x310e](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/DiagramHTCTitanx310e.png)

### Nokia Lumia 800, Windows Phone 7.5

![Results Nokia Lumia 800](https://raw.github.com/divineprog/MoSyncApps/master/JSFrameworkBenchmark/Results/DiagramNokiaLumia800.png)

## Discussion

Results show that jqMobi outperforms the other libraries, being much faster than both jQuery and Zepto for DOM operations, and loading slightly faster. jQuery is in general faster for DOM operations than Zepto, but for some operations Zepto performs better. Zepto loads slightly faster than jQuery.

One interesting result is that on iPad 2, Zepto is about two seconds slower for the Create DOM test, but with jqMobi, performance is about equal on the two devices. This indicates that jqMobi manages to get good performance out of iPad 2 (another way to put it is that jqMobi fails to get increased performance on iPad 3 ;-)

Page loading time on iOS is faster when relaunching the app, because of the way iOS caches pages. For example, in our test on iPhone 4S, jqMobi loaded in 843 ms on first app start, and next time it loaded in 399 ms. Note that the diagrams show the average of first and second load time. The same observation was made on Windows Phone, second page load was up to twice as fast as the first one. On all Android devices tested, page load time did not vary significantly between launches.

We also did some tests using the non-minified versions of the libraries, and the results show that while loading time is marginally slower, the performance is equal to the minified versions. Thus, for an app that bundles files with the application package, there does not seem to be any performance gain in using a minified library.

For DOM operations, Windows Phone devices are considerably slower than Android and iOS devices. In the Create DOM test, the performance difference is substantial, and the Windows Phone devices tested are outperformed by for instance Google Nexus One, launched in January 2010, and a low-end device like HTC Wildfire S. This indicates that Internet Explorer performance has room for improvement to catch up with the WebKit-based browsers used on Android and iOS.

Depending of the characteristics of your application, the performance of DOM operations may not be very critical. However, if your app contains of a large number of DOM elements, the choice of library can have a big impact of performance. 
