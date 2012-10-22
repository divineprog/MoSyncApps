<!-- # Using The MoSync SQL API in Hybrid Apps  -->
<!-- C:\md>perl Markdown.pl C:\MoSyncProjects\MoSyncApps\HybridSQLDemo\Tutorial\HybridSQLTutorial.md > output.txt -->

<!--
<style type="text/css">
p>img {
  width: 550px;
}
</style>
-->

This tutorial demonstrates how to use the MoSync Database API in a Hybrid HTML/JavaScript/C++ application. In the [previous tutorial on Web SQL](http://www.mosync.com/documentation/manualpages/using-web-sql-mosync-apps), we made a simple game in plain HTML5/JavaScript. This time, we will use C++ to implement the database functionality of the application, while reusing the JavaScript user interface code from the Web SQL example. We will discuss design considerations for hybrid apps, and go into the implementation details of how to invoke C++ code from JavaScript.

The source code for this tutorial is available on [GitHub](https://github.com/divineprog/MoSyncApps/tree/master/HybridSQLDemo).

## Background

HTML5 has many useful features, but there can be times when you miss someting. By making a hybrid app, which combines JavaScript with native code, the missing functionality can be made available. In the case of the MoSync SDK, functionality not available in JavaScript can be implemented in C/C++.

MoSync has a C/C++ Database API (based on SQLite). Similar to Web SQL, this API provides functions for creating and querying SQL databases. To access the C/C++ Database API from JavaScript, we will use a binding mechanism available in the MoSync Wormhole API, which makes it straightforward to invoke C++ functions from JavaScript. You can use this invokation mechanism to add any custom C++ code to your hybrid app, not just database-related functionality.

Compared to the Web SQL API, the MoSync Database API gives you greater flexibility and a wider range of functionality. The Web SQL standard is currently not actively maitained by the W3C Working Group, which makes the MoSync Database API an attractive option, an independent database API you can use in your hybrid apps.

## Demo application

The app HybridSQLDemo is a simple turn-based game of luck (much like playing by tossing a dice). The "dice" in the app is a "wheel" with numbers 1 tho 50. Whoever gets the highest number wins the current round, and the total score gets updated. This is the same app as we used in the [Web SQL tutorial](http://www.mosync.com/documentation/manualpages/using-web-sql-mosync-apps), but with a different database implementation.

You challenge the app and can affect the outcome by taking a low risk or a high risk. With high risk, you can double your score, but the app gets to roll the wheel twice.

Here is a screenshot:

![Hybrid SQL Demo Screenshot](https://raw.github.com/divineprog/MoSyncApps/master/HybridSQLDemo/Tutorial/HybridSQLDemo.png)

The source code is available on GitHub. The JavaScript layer of the app is in file [index.html](https://github.com/divineprog/MoSyncApps/blob/master/HybridSQLDemo/LocalFiles/index.html). The  C++ layer is in file [main.cpp](https://github.com/divineprog/MoSyncApps/blob/master/HybridSQLDemo/main.cpp).

The database is used to store the total score of the two players (the user and the app). This makes the score persistent. While not a big database with lots of objects, it shows how to perform basic database operations, like querying and updating rows.

## Layered architectures

When using a layered architecture, like the Wormhole JavaScript/C++ framework, there is always the question of what to put in the different layers. In the case of the database functionality needed for our simple game app, we have at least two, quite different, options:

* Implement a general JavaScript SQL API (usable with any app)
* Implement application specific services made available from JavaScript

These design alternatives exist, not only for the database API, but for almost any functionality in your app that you implement using a layered/hybrid approach. It is a question of how to divide the code between the JavaScript layer and the C++ layer.

The first approach, to implement an API, would be similar to implementing the Web SQL API on top of MoSync, replacing the browser's implementation with our own MoSync version. This approach is of course the given choise if we would wish to make a generally reusable API that can be used in a wide spectrum of applications. But there are also drawbacks with going for the general solution.

The time it takes to implement a generic library can be considerable. Either you have to implement an existing specification, which can be tricky to get right. And if you design your own API, the design itself can take time to get right. Then there is the need for testing, documenting and maintaining the API, all of which can steal time from actual application development. The risk is that your project will end up implementing an API, not an app! ;)

The second approach, to implement the specific services needed by the application, is less general, but can be quicker to design, code, test, document and maintain. In our example, by services are functionality provided by the C++ layer. 

For example, if we wish to get the current score of the player, here is the code we could write when using a JavaScript database API (like Web SQL):

    function getScore(name, fun)
    {
        db.execSQL("SELECT score FROM player WHERE name=?", [name],
            function(result)
            {
                var row = result.rows.item(0);
                fun(row.score);
            });
    }

By comparison, here is the JavaScript code we could write when invoking a C++ service-oriented API:

    function getScore(name, fun)
    {
        mosync.bridge.send(["Custom", "GetScore", name], fun);
    }

In this approach, we don't care about the details of how database queries are performed. All we ask for, is for our callback function to get called with the score of the player with the name we provide. The details are taken care of by the C++ layer.

While the service-oriented approach usually is very application specific, it is a solution that is straightforward to implement and maintain. Furthermore, there can be performace issues to consider. By reducing the number of calls made between JavaScript and C++, performance can be improved. And code implemented in C++ could be faster than the corresponding JavaScript implementation would be. 

For the simplicity and usefullness of the service-oriented design, we will use this appraoch in the example app of this tutorial.

## A tour of the MoSync Database C API

MoSync SDK supports the SQL API on iOS and Android. The APi is based on SQLite. Database files are stored in the local files system of the device. An overview of the API is given in the [Database C API guide](http://www.mosync.com/documentation/manualpages/sql-database-api). Additional details are provides in the [Database API documentation](http://www.mosync.com/files/imports/doxygen/latest/html/group___d_b_a_p_i.html).

Here is a quick tour of the API.

Use the global function maDBOpen to create/open a database:

    MAHandle db = maDBOpen(databasePath);

Use the maDBExecSQL function to execute queries:

    maDBExecSQL(db, "CREATE TABLE pet (name TEXT(50), age INTEGER)");
    maDBExecSQL(db, "INSERT INTO pet VALUES ('Charmy', 7)");
    maDBExecSQL(db, "INSERT INTO pet VALUES ('Vilma', 12)");

For queries that return a result set, a cursor is used to iterate over the query result:

    char name[64];
    MAHandle cursor = maDBExecSQL(db, "SELECT * FROM pet");
    printf("All my pets:\n");
    while (MA_DB_OK == maDBCursorNext(cursor))
    {
        // Read data into the name buffer.
        int size = maDBCursorGetColumnText(cursor, 0, name, 64);
        // Zero-terminate the name string.
        name[size] = 0;
        // Print the name.
        printf("%s\n", name);
    }

Release the cursor when we are done with it:

    maDBCursorDestroy(cursor);
    
Finally close the database:

    maDBClose(db);

Note that the above code contains no error checking, see the [Database C API guide](http://www.mosync.com/documentation/manualpages/sql-database-api) for further details on how to handle errors.

## How the JavaScript to C++ bridge works

The recent nightly builds of the MoSync SDK has a new binding mechanism that makes it easy to invoke C++ functions from JavaScript. This mechanism will also be available in the upcoming release of MoSync SDK 3.2. If you want to use the mechanism available in MoSync SDK 3.1, check out the tutorial [Extending HTML5 Apps with C++](http://www.mosync.com/documentation/manualpages/how-communicate-between-javascript-and-c-mosync). _Note that the code in this tutorial uses the new mechanism_ (only available in nightly builds until MoSync SDK 3.2 is released).

From JavaScript we use mosync.bridge.send to invoke C++ code. This function takes two parameters, an array of strings, and an optional callback function. When using the new binding mechanism, the first string in the array always must be "Custom" (indicating a custom call is being made). Here is an example:

    mosync.bridge.send(["Custom", "Vibrate"]);
    
In the C++ code, the "Vibrate" message is bound to a C++ function using the following code:

		addMessageFun(
			  "Vibrate",
			  (FunTable::MessageHandlerFun)&MyMoblet::vibrate);

This will call the function _vibrate_ in class MyMoblet when the message "Vibrate" is sent from JavaScript. The function pointer type _FunTable::MessageHandlerFun_ is used for all functions invoked using this mechanism.

And here is how the _vibrate_ function can be implemented:

    void MyMoblet::vibrate()
    {
        maVibrate(500);
    }

The example program HybridSQLDemo uses this mechanism. Look at the source code in the file [main.cpp](https://github.com/divineprog/MoSyncApps/blob/master/HybridSQLDemo/main.cpp) to examine the details. 

## Implementation of database servcies

In the file [main.cpp](https://github.com/divineprog/MoSyncApps/blob/master/HybridSQLDemo/main.cpp) you will also find the implementation of the database services used by the application.

As an example, here is how the "GetScore" service is implemented:

