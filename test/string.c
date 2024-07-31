#include "w/t35t/assert.h"
#include "w/t35t/test.h"

#include "../string_.h"

TestResult
TestEquality(TestContext *tc) {
	String a = S_Wrap("aaa");
	String b = S_Wrap("aaa");
	String c = S_Wrap("ccc");

	AssertEquals(a, b);
	AssertNotEquals(a, c);
	AssertNotEquals(b, c);

	return TEST_OK;
}

TestResult
TestFormat(TestContext *tc) {
	String a = S_Format("%s is no %s", "There", "spoon");
	String b = S_Format("Rule of %d", 3);

	AssertEquals(b, S_Wrap("Rule of 3"));
	AssertEquals(a, S_Wrap("There is no spoon"));

	return TEST_OK;
}

int main() {
	TestContext tc = tTC_Alloc();

	tTC_AddTest(&tc, TestEquality);
	tTC_AddTest(&tc, TestFormat);
	return tTC_Engage(&tc);
}
