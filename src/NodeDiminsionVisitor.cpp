#include "NodeDiminsionVisitor.h"
#include "SumNode.h"
#include <assert.h>
#include "wx/log.h"
#include "Layout.h"

CNodeSizeVisitor::CNodeSizeVisitor(wxDC& arDC, CLayout& arLayout)
        : mrDC(arDC),
          mrLayout(arLayout),
          mRank(0),
          mDirection(ENone)
{
        mNodeStack.push(0);
}

CNodeSizeVisitor::~CNodeSizeVisitor()
{
}



void CNodeSizeVisitor::Accept(CSumNode& arSumNode)
{
        wxString Value = GetNodeText(arSumNode);


        int TextHeight;
        int TextWidth;
        mrDC.GetTextExtent(Value, &TextWidth, &TextHeight);

        CNodeInfo Node(Value,
                       TextWidth, 
                       arSumNode.GetType(),
                       arSumNode.IsNodeCollapsed(),
                       &arSumNode);


        mNodeSizes.insert(std::make_pair(&arSumNode, wxPoint(TextWidth,TextHeight)));

        CNodeInfo* pAddedNode = mrLayout.AddNode(Node, mRank);

        CNodeInfo* pParent = mNodeStack.top();

        if (pParent)
        {
                pAddedNode->SetParent(pParent);

                if(mDirection == ELeft) 
                        pParent->SetLeftChild(pAddedNode);
                if(mDirection == ERight) 
                        pParent->SetRightChild(pAddedNode);
        }

        mNodeStack.push(pAddedNode);

        if (!arSumNode.IsLeaf())
        {
                ++mRank;
                mDirection = ELeft;
                Accept(arSumNode.GetLeft());

                mDirection = ERight;
                Accept(arSumNode.GetRight());
                --mRank;
        }

        mNodeStack.pop();
}

const wxPoint& CNodeSizeVisitor::GetPoint(const CSumNode& arSumNode) const
{
        std::map<const CSumNode*,wxPoint>::const_iterator It =  mNodeSizes.find(&arSumNode);

        assert(It != mNodeSizes.end());

        return It->second;
}

int CNodeSizeVisitor::GetWidth(const CSumNode& arSumNode) const
{
        return GetPoint(arSumNode).x;
}

int CNodeSizeVisitor::GetHeight(const CSumNode& arSumNode) const
{
        return GetPoint(arSumNode).y;
}
