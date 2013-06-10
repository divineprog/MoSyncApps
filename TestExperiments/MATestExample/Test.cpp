#include <conprint.h>
#include <matime.h>
#include <mavsprintf.h>
#include "MAUtil/String.h"
#include "Test.h"

namespace MATest {

	using namespace MAUtil;

	void TestListener::beginTestSuite(const String& suiteName) {
	}

	void TestListener::endTestSuite(){
	}

	void TestListener::beginTestCase(const String& testCaseName){
	}

	void TestListener::endTestCase(){
	}

	void TestListener::assertion(const String& assertionName, bool cond){
	}

	void TestListener::expect(const String& assertionName){
	}

	TestListener::~TestListener() {
	}

	TestCase::TestCase(const String& name) : name(name) {
	}

	TestCase::~TestCase() {
	}

	void TestCase::open() {
	}

	void TestCase::close() {
	}

	bool TestCase::assert(const String& assertionName, bool cond) {
		suite->fireAssertion(assertionName, cond);
		return cond;
	}

	void TestCase::expect(const String& assertionName) {
		suite->fireExpect(assertionName);
	}

	const String& TestCase::getName() const {
		return name;
	}

	void TestCase::setSuite(TestSuite *suite) {
		this->suite = suite;
	}

	TestSuite* TestCase::getSuite() {
		return this->suite;
	}

	void TestCase::runNextTestCase()
	{
		getSuite()->runNextCase();
	}

	TestSuite::TestSuite(const String& name) : name(name), mNextCase(0) {
	}

	TestSuite::~TestSuite() {
	}

	void TestSuite::addTestCase(TestCase *testCase) {
		testCases.add(testCase);
		testCase->setSuite(this);
	}

	void TestSuite::runNextCase() {
		if(mNextCase == 0) {
			for(int j = 0; j < testListeners.size(); j++) {
				testListeners[j]->beginTestSuite(name);
			}
		} else {
			testCases[mNextCase - 1]->close();
			for(int j = 0; j < testListeners.size(); j++) {
				testListeners[j]->endTestCase();
			}
		}
		if(mNextCase == testCases.size()) {
			for(int j = 0; j < testListeners.size(); j++) {
				testListeners[j]->endTestSuite();
			}
			return;
		}

		int curCase = mNextCase++;
		for(int j = 0; j < testListeners.size(); j++) {
			testListeners[j]->beginTestCase(testCases[curCase]->getName());
		}
		testCases[curCase]->open();
		testCases[curCase]->start();
	}

	const String& TestSuite::getName() const {
		return name;
	}

	void TestSuite::addTestListener(TestListener *testListener) {
		testListeners.add(testListener);
	}

	void TestSuite::fireAssertion(const String& str, bool cond) {
		for(int j = 0; j < testListeners.size(); j++) {
			testListeners[j]->assertion(str, cond);
		}
	}

	void TestSuite::fireExpect(const String& str) {
		for(int j = 0; j < testListeners.size(); j++) {
			testListeners[j]->expect(str);
		}
	}
}
// namespace
