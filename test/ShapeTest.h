#ifndef SHAPETEST_H
#define SHAPETEST_H
#include <cppunit/TestCase.h>
// Author Martin Bradley.

class CShapeTest : public CppUnit::TestCase
{
public:
        CShapeTest();

        static CppUnit::Test* suite();
        void setUp();

protected: 

        void testBoxShapeConstructorTooManyCoordinates();
        void testBoxShapeContainsMethod();
        void testBoxShapeConstructorNonNumeric();

private:
};
#endif // SHAPETEST_H
