#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/TestResult.h>
#include "ShapeTest.h"
#include "BoxShape.h"
#include <stdexcept>

CShapeTest::CShapeTest()
{
}

void CShapeTest::testBoxShapeConstructorTooManyCoordinates()
{
        // Check that it throws an exception if too many arguments are given.
        CPPUNIT_ASSERT_THROW_MESSAGE("Need four coordinates",
                                     CBoxShape("54,3,32,126,54"),
                                     std::invalid_argument);
}

void CShapeTest::testBoxShapeConstructorNonNumeric()
{
        // Check that it throws an exception if non double values are used.
        CPPUNIT_ASSERT_THROW_MESSAGE("Need double values for the coordinates",
                                     CBoxShape("f,32,16,54"),
                                     std::invalid_argument);

        // Check that it throws an exception if non double values are used.
        CPPUNIT_ASSERT_THROW_MESSAGE("Need double values for the coordinates",
                                     CBoxShape("3,32,z6,54"),
                                     std::invalid_argument);
}

void CShapeTest::testBoxShapeContainsMethod()
{
        CBoxShape BoxShape("54,6,126,54");
        bool Result = BoxShape.Contains(CPoint(80,40));

        CPPUNIT_ASSERT(Result == true);

        Result = BoxShape.Contains(CPoint(54,6));

        CPPUNIT_ASSERT(Result == true);

        Result = BoxShape.Contains(CPoint(154,6));

        CPPUNIT_ASSERT(Result == false);

        Result = BoxShape.Contains(CPoint(0,0));

        CPPUNIT_ASSERT(Result == false);

        Result = BoxShape.Contains(CPoint(-10,0));

        CPPUNIT_ASSERT(Result == false);

        Result = BoxShape.Contains(CPoint(80,40));

        CPPUNIT_ASSERT(Result == true);
}



CppUnit::Test* CShapeTest::suite()
{
        CppUnit::TestSuite* suiteOfTests = new CppUnit::TestSuite("CBoxShapeTest");


        suiteOfTests->addTest(new CppUnit::TestCaller<CShapeTest>(
                              "testBoxShapeConstructorTooManyCoordinates",
                              &CShapeTest::testBoxShapeConstructorTooManyCoordinates));

        suiteOfTests->addTest(new CppUnit::TestCaller<CShapeTest>(
                              "testBoxShapeContainsMethod",
                              &CShapeTest::testBoxShapeContainsMethod));

        suiteOfTests->addTest(new CppUnit::TestCaller<CShapeTest>(
                              "testBoxShapeConstructorNonNumeric",
                              &CShapeTest::testBoxShapeConstructorNonNumeric));



        return suiteOfTests;
}

void CShapeTest::setUp()
{
}

int main ()
{
        CppUnit::TextTestRunner runner;
        runner.addTest(CShapeTest::suite());
        runner.run();
        return 0;
}
