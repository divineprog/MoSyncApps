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

The test case app is called TwitterReader, a JS/C++ hybrid app that displays Twitter feeds.

For each framework tested, we implement the UI of the app using that framwork. Then we run the  app on different devices, and evaluate it using the criteria described below.

The evaluation critera we use are based on (1) the user experience, and (2) the developer experience.

User experience critera:

* Graphical appearance (aesthetic impression, does the UI look "good"?)
* Performance (how "snappy" is the UI)

Developer experience criteria:

* Ease of learning and using the framework
* Functionality (are the features needed to implement the TwitterReader UI available?)

The above are to a large extent subjective criteria. But you don't have to accept our view. The source code is available, and we provide screenshots and videos, so you can judge yourself.

Note that we apply these criteria to the test app. Our evaluation is best characterised as a case based study. We do not capture every aspect of the libraries, but we capture the aspects relevant for the TwitterReader app.

To make results a bit more concrete, we will grade each critera on a scale from 1 to 5, where 1 is the worst, and 5 is the best. Here are the grading critera:

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

1. The framework is impossible to understand and learn for the purpose of implementing the test app.
2. It is possible to learn and use some parts of the framework, but documentation and examples are very poor, and there are many aspects that are not understandable.
3. It is possible to learn and use the framework with some effort, but some parts needed for the test app require lots of time to understand and use.
4. It is easy to learn and use all of the framework, with only a few parts requiring additional time to use.
5. It is very easy to learn and use the framework to tmplement the test app, finding documentation and examples is very easy.

The critera are quite closely related to the implementation of the TwitterReader example app. But, this app is quite basic, and uses UI elements common to many applications, so it have some relevance for a wider range of applications. Of course, the ease of learning and use is closely related to developer experience, both in general, and experience with similar frameworks.

Functionality:

1. The UI of the app cannot be implemented at all.
2. Not all parts of the UI could be implemented.
3. The UI of the app works, but there are several features in the UI missing.
4. All parts of the UI could be implemented, with only some small features are missing.
5. All of the UI features could be fully implemented.

These critera relates specifically to the TwitterReader example app. A user interface that has the UI features of the original jqTouch UI would get 5 on this scale. This criteria is also related to developer experience. A skilled developer who knows the UI framework well, could probably implement missing functionality.

## Overview of the test app

The TwitterReader app is based on the example app WebViewTwitter, which is shipped with the MoSync SDK. The app is a JS/C++ hybrid app that displays Twitter feeds. The source code is available on [GitHub](https://github.com/divineprog/MoSyncApps/tree/master/TwitterReader).

### User interface

Here are screenshots of the original version of the app, which is implemented using the jqTouch library.

You enter a Twitter use name, and the recent tweets for that user is displayed on a separate screen. You can also save the user name in a list of favourite users, and later use that as a shortcut. Favourites are stored on the device in a text file (no method for removing items from the list is currently provided).

### Architecture

Since there are several versions of the app implemented using different UI libraries, the app use a layered architecture (which is a goo idea anyway). The following layers are used:

* Application layer (shared by all versions)
* General UI layer (shared by all versions)
* Framework specific layer (not shared)

Using this approach, only one layer needs to be rewritten/updated when a new UI library is used.

### In the code

Shared files:

* app.js - Contains the app object
* app.ui.js - Contains the app.ui object
* main.cpp - 

Framework specific file:

* index.html

## Frameworks tested

For Part 1, we have tested jqTouch and iUI.


## Example app architecture


## jQTouch

### Look (aesthetic impression)

### Performance (how "snappy" is the UI)

### Ease of understanding the framework

### Functionality
