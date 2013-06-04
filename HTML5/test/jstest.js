// ****** Buster Library ******

buster = {};
buster.testCases = {};
buster.numberOfTestCases = 0;
buster.numberOfDoneTestCases = 0;
buster.numberOfFails = 0;

/*mosync.rlog = function(m)
{
	document.body.innerHTML =
		m + "<br/>" +
		document.body.innerHTML;
};*/

buster.runAllTestCases = function()
{
    for (var key in this.testCases) 
    {
        var test = this.testCases[key];
        test.fun(test);
    }
};

buster.testCase = function (name, fun)
{
    this.testCases[name] = 
    { 
        "name": name
        };
    this.numberOfTestCases ++;
};

buster.testCase = function (name, fun)
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

buster.testCase("TimerTest", function(test)
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

buster.testCase("NativeUITest", function(test)
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

buster.testCase("FailingTest", function(test)
{
    test.shouldHold(false, 'FailingTest1');
    test.done();
});

buster.runAllTestCases();