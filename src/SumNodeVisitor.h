#ifndef SUMNODEVISITOR_H
#define SUMNODEVISITOR_H
#include "SumNode.h"

class CSumNodeVisitor
{
public:
    virtual ~CSumNodeVisitor() = 0;

    virtual void Accept(CSumNode& arSumNode) = 0;
};
#endif // SUMNODEVISITOR_H
