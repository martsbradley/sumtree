#ifndef SHAPETEST_H
#define SHAPETEST_H
#include <cppunit/TestCase.h>
// Author Martin Bradley.

class CStringTest : public CppUnit::TestCase
{
public:
        CStringTest();

        static CppUnit::Test* suite();
        void setUp();

protected: 

        void testTrimWhiteSpace();

private:
};
#endif // SHAPETEST_H
