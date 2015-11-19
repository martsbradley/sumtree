#ifndef NODEDIMINSION_VISITOR_H
#define NODEDIMINSION_VISITOR_H
#include "SumNodeVisitor.h"
#include "wx/dc.h"
#include "wx/gdicmn.h"
#include <map>
#include <stack>

class CLayout;
class CNodeInfo;
enum EDirection
{
        ENone,
        ELeft,
        ERight
};

class CNodeSizeVisitor : public CSumNodeVisitor
{
public:
        CNodeSizeVisitor(wxDC& arDC, CLayout& arLayout);

        virtual ~CNodeSizeVisitor();
        virtual void Accept(CSumNode& arSumNode);

        int GetWidth(const CSumNode& arSumNode) const;
        int GetHeight(const CSumNode& arSumNode) const;

private:
        const wxPoint& GetPoint(const CSumNode& arSumNode) const;
        std::map<const CSumNode*,wxPoint> mNodeSizes;
        wxDC& mrDC;
        CLayout& mrLayout;
        std::stack<CNodeInfo*> mNodeStack;
        size_t mRank;
        EDirection mDirection;
};
#endif // NODEDIMINSION_VISITOR_H
