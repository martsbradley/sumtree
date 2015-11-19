#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/TestResult.h>
#include "StringTest.h"
#include "wstringconvert.h"
#include <stdexcept>

CStringTest::CStringTest()
{
}

void CStringTest::testTrimWhiteSpace()
{
        std::string String(" test this ");

        String = trimString(String);
        CPPUNIT_ASSERT(String == "test this");

        String = "\"test this\"";

        String = trimString(String," \t\"");
        CPPUNIT_ASSERT(String == "test this");
}



CppUnit::Test* CStringTest::suite()
{
        CppUnit::TestSuite* suiteOfTests = new CppUnit::TestSuite("CStringTest");


        suiteOfTests->addTest(new CppUnit::TestCaller<CStringTest>(
                              "testTrimWhiteSpace",
                              &CStringTest::testTrimWhiteSpace));
        return suiteOfTests;
}

void CStringTest::setUp()
{
}

int main ()
{
        CppUnit::TextTestRunner runner;
        runner.addTest(CStringTest::suite());
        runner.run();
        return 0;
}
