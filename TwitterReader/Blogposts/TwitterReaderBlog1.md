# In Search Of The Perfect Mobile JavaScript UI Framework - Part 1 (jqTouch, iUI)

<!-- C:\md>perl Markdown.pl C:\MoSyncProjects\MoSyncApps\TwitterReader\Blogposts\TwitterReaderBlog1.md > C:\MoSyncProjects\MoSyncApps\TwitterReader\Blogposts\TwitterReaderBlog1.html -->

<!--
<style type="text/css">
p>img {
  width: 550px;
}
</style>
-->

Previously on Codefessions we did an evaluation of JavaScript user interface frameworks for mobile applications ([Part 1](http://www.codefessions.com/2012/04/mobile-javascript-frameworks-evaluation.html), [Part 2](http://www.codefessions.com/2012/04/which-mobile-javascript-framework-is.html), [Part 3](http://www.codefessions.com/2012/05/which-mobile-javascript-framework-is.html)). Now it is time to revisit this subject. There are more user interface frameworks than ever to choose from that target mobile devices. Join in on our search for the perfect Mobile JavaScript UI Framework :)

## Method

This is the first in a series of blog posts where we evaluate JavaScript UI frameworks for mobile applications. 

The approach we are using is based on taking a mobile hybrid app and implement different versions of it using different user interface libraries. That is, we are using the same app as a test case for the various UI frameworks we are evaluating.

The test case app is called TwitterReader, a MoSync HTML5/JS app that displays Twitter feeds. This app is based on the [WebViewTwitter](https://github.com/MoSync/MoSync/tree/master/examples/html5/WebViewTwitter) example app that comes with the [MoSync SDK](http://www.mosync.com/sdk). Orignally this app used the [jQTouch](https://github.com/senchalabs/jQTouch) UI library.

For each framework tested, we implement the UI of the app using that framwork. Then we run the app on different devices, and evaluate it using the criteria described below.

The evaluation method is simular to [heuristic evaluation](http://en.wikipedia.org/wiki/Heuristic_evaluation), and the evaluation criteria (heuristics) we use are based on (1) the end-user experience, and (2) the developer experience.

User experience critera:

* Graphical appearance (aesthetic impression, does the UI look "good")
* Performance (how "snappy" is the UI)

Developer experience criteria:

* Ease of learning and using the framework
* Functionality (are the features needed to implement the TwitterReader UI available?)

Technical critera:

* Framework support (which platforms does the framwork run on)

The above criteria are to a large extent based on subjective assessments. But you don't have to accept our view. The source code is available, and we provide screenshots and videos, so you can judge yourself.

Note that we apply the evaluation criteria to the test app we use. Our evaluation is best characterised as a case based study. We do not capture every aspect of the UI frameworks, but we capture the aspects relevant for the TwitterReader app.

To make results a bit more concrete, we will grade each critera on a scale from 1 to 5, where 1 is the worst, and 5 is the best (again these are subjective values). Here are the grading critera:

Graphical appearance:

1. The UI is broken and does not display properly.
2. The UI has substantial problems, graphical elements misalign, elements do not display properly.
3. The UI looks acceptable, but has some glitches that makes it look unfinished.
4. The UI looks good, and has no major glitches.
5. The UI looks professionally designed, follows established guidelines for design (not neccecarily the platform's UI guidelines)

A graphical apperance that looks like the platform's native UI would get 5 as a score.

Performance:

1. The UI cannot be used to perform the functionality of the app.
2. The UI has substantial performace problems, with heavy lagging.
3. The UI has acceptable performance, but feels a bit slow.
4. The UI has good performance, only minor performance glitches.
5. The UI has fully fluid performance.

A performance similar to that of the platform's native UI would get 5 as a score.

Ease of learning and using the framework:

1. It is impossible to understand and learn the framework for the purpose of implementing the test app.
2. It is possible to learn and use some parts of the framework, but documentation and examples are very poor, and there are many aspects that are not understandable.
3. It is possible to learn and use the framework with some effort, but some parts needed for the test app require lots of time to understand and use.
4. It is easy to learn and use all of the framework, with only a few parts requiring additional time to use.
5. It is very easy to learn and use the framework to tmplement the test app, finding documentation and examples is very easy.

These critera are quite closely related to the implementation of the TwitterReader example app. But, the app is quite basic, and uses UI elements common to many applications, so it has some relevance for a wider range of applications. Regarding ease of learning and use, this is of course closely related to developer experience, both in general, and in experience with similar frameworks.

Functionality:

1. The UI of the app cannot be implemented at all.
2. Not all parts of the UI can be implemented.
3. The UI of the app works, but there are several features in the UI missing.
4. All parts of the UI can be implemented, with only some small features are missing.
5. All of the UI features can be fully implemented.

Again, the evaluation critera relate to the TwitterReader example app. A user interface that has the UI features of the original jQTouch UI would get 5 on this scale. This criteria is also related to developer experience. A skilled developer who knows the UI framework well, could probably implement missing functionality.

Framework support:

For framework support we list the platforms/versions the framework runs on. 

Tested platforms/devices:

* Android 2.3 (Nexus One)
* Android 4.2 (Nexus 7)
* iOS 5 (iPhone 4)
* Windows Phone 8 (HTC?)

This gives a score from 0 to 4 (0 = does not run on any platform, 4 = runs on all 4 platforms).

## Overview of the test app

The TwitterReader app is based on the example app WebViewTwitter, which is shipped with the MoSync SDK. The app is a HTML5/JS app that displays Twitter feeds.

Here are screenshots of the original version of the app, which is implemented using the jQTouch library.

TODO: Add screenshots.

You enter a Twitter use name, and the recent tweets for that user is displayed on a separate screen. You can also save the user name in a list of favourite users, and later use that as a shortcut. Favourites are stored on the device using HTML5 Local Storage (no method for removing items from the list is currently provided).

## In the code

The source code for the test apps are available on [GitHub](https://github.com/divineprog/MoSyncApps/tree/master/TwitterReader).

### Architecture

Since there are several versions of the app implemented using different UI libraries, we use a layered architecture (which is a good idea anyway). The following layers are used (implemented in JavaScript):

* Application layer (shared by all versions)
* General UI layer (shared by all versions)
* Framework specific UI layer (not shared)

The appication layer contains logic for the application. The UI layer is sprit in two parts, a general part and a specific part. Using this approach, only one layer needs to be rewritten/updated when a new UI library is used. 

### Files

Shared files:

* [app.js](https://github.com/divineprog/MoSyncApps/tree/master/TwitterReader/Common/js/app.js) - Contains the app object (application layer)
* [app.ui.js](https://github.com/divineprog/MoSyncApps/tree/master/TwitterReader/Common/js/app.ui.js) - Contains the app.ui object (https://github.com/divineprog/MoSyncApps/tree/master/TwitterReader/Common/js/app.js) (general UI layer)
* [wormhole.js](https://github.com/divineprog/MoSyncApps/tree/master/TwitterReader/Common/js/wormhole.js) - Contains the app.ui object (https://github.com/divineprog/MoSyncApps/tree/master/TwitterReader/Common/js/wormhole.js) (general UI layer)
* [main.cpp](https://github.com/divineprog/MoSyncApps/tree/master/TwitterReader/Common/cpp/main.cpp) - Start up code that creates the app

Framework specific files:

* index.html - Contains framwork specific code added to the app.ui object (see code for this file in the specific projects, each project has its own version)

Similar to how class-based languages use overriding of methods in a superclass to implement specific behaviour, the JavaScript code in index.html replaces/fills in functions in the app.up object. Browse the code to see how this is done.

For Part 1, we have tested jqTouch and iUI, here are the projects and files:

* [TwitterReaderJQTouch](https://github.com/divineprog/MoSyncApps/tree/master/TwitterReader/TwitterReaderJQTouch) - [index.html](https://github.com/divineprog/MoSyncApps/tree/master/TwitterReader/TwitterReaderJQTouch/LocalFiles/index.html)
* [TwitterReaderIUI](https://github.com/divineprog/MoSyncApps/tree/master/TwitterReader/TwitterReaderIUI) - [index.html](https://github.com/divineprog/MoSyncApps/tree/master/TwitterReader/TwitterReaderIUI/LocalFiles/index.html)

## jQTouch

The first framework tested is [jQTouch](http://www.jqtouch.com/), Version 1.0 beta 4rc. jQTouch is the framework originally used for the WebViewTwitter example application that ships with the MoSync SDK.

### Look (aesthetic impression)

TODO: Screenshots.

Score: 4

### Performance (how "snappy" is the UI)

### Ease of understanding the framework

### Functionality

Total score for jQTouch: X

## iUI

### Look (aesthetic impression)

TODO: Screenshots.

### Performance (how "snappy" is the UI)

### Ease of understanding the framework

### Functionality

## Conclusion

To be continued.
