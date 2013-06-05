// ****** mosync.test Library ******

if (typeof mosync === "undefined") { mosync = {}; }
mosync.test = {};
mosync.test.testCases = {};
mosync.test.numberOfTestCases = 0;
mosync.test.numberOfDoneTestCases = 0;
mosync.test.numberOfFails = 0;

// mosync.rlog = console.log;
/*mosync.rlog = function(m)
{
	document.body.innerHTML =
		m + "<br/>" +
		document.body.innerHTML;
};*/

mosync.test.runAllTestCases = function()
{
    var me = this;
    for (var key in me.testCases) 
    {
        var test = me.testCases[key];
        test.fun(test);
    }
};

mosync.test.testCase = function (name, fun)
{
    var me = this;
    this.testCases[name] = 
    { 
        "name": name,
        "fun": fun,
        "done": function()
        {
            me.numberOfDoneTestCases ++;
            if (me.numberOfDoneTestCases == me.numberOfTestCases)
            {
                mosync.rlog("All tests done (fails: " + me.numberOfFails + ")");
            }
        },
        "shouldHold": function(cond, label)
        {
            if (!cond)
            {
                me.numberOfFails ++;
                mosync.rlog("Test failed: " + label);
            }
            else
            {
                mosync.rlog("Test passed: " + label);
            }
        }
    };
    me.numberOfTestCases ++;
};

// ****** Test Cases ******

mosync.test.testCase("TimerTest", function(test)
{
    var time = Date.now();
    var delay = 2000;
    setTimeout(function()
    {
        test.shouldHold(Date.now() >= time + delay, 'TimerTest1');
        test.done();
    },
    delay);
});

mosync.test.testCase("NativeUITest", function(test)
{
    function createScreen()
    {
        mosync.nativeui.create
        (
            "Screen", 
            "MyScreen",
            {
                "title": "My Screen"
            },
            createButton
        );
    }
    
    function createButton()
    {
        mosync.nativeui.create
        (
            "Button", 
            "MyButton",
            {
                "width": "FILL_AVAILABLE_SPACE",
                "text": "Say Hello"
            },
            showButton
        );
    }
    
    function showButton()
    {
        var screen = document.getNativeElementById("MyScreen");
        test.shouldHold(screen != null, "NativeUITest1");
        screen.addChild("MyButton");
        screen.show();
        test.done();
    }
    
    createScreen();
});

mosync.test.testCase("FailingTest", function(test)
{
    test.shouldHold(false, 'FailingTest1');
    test.done();
});

mosync.test.runAllTestCases();
