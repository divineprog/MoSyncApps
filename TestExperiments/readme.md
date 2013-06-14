# Test Experiments

## Overview

This repository contains test framework experiments.

## MATestExample

This example uses the existing MATest framework. What is added is a class TestRunner (with a different implementation compared to the other examples), which simplifies registering and running tests a bit. It also includes a sample TestListener (SimpleTestListener) that prints the test result to the screen. This could be modified to log results on file or send over the network. There is support for Expect and for timeout of tests that take too long to run. NOTE: This code handles static initialisation in a safe way, but the order in which test cases defined in multiple files are run is non-deterministic.

## OldBuggyExperiments

Code that is now outdated.

# Requreiments

For a list of requirements, see this MoSync Jira story: http://jira.mosync.com/browse/MOSYNC-2987

# Discussion

Regarding the use of functions vs. classes for defining test cases, classes have some advantages.
Classes can define multiple functions in the same "namespace", hold data, maintain state, implement listeners, etc. MATest uses this approach, and this is used by MATestExample. For short test cases, functions are convinient, for more complext test cases, classes are propably a better option. For short test cases, a test case class could be created, and within that class, several test functions could de defined and called from the test case run method (called start in MATest).

What I like about MATest is that there are no macros, and no global objects (singletons). This makes for a clean code structure with few assumptions. A singleton is however added in the above example, to simplify registration of test cases. You are however free to not use this mechanism if you wish, and create/register/run test cases in your own initialisation code.

