#include "SumNodeExpanderVisitor.h"
#include <assert.h>

CSumNodeExpanderVisitor::CSumNodeExpanderVisitor(const wxString& arNodeToExpandOrContract,
                                                 bool aJustFindTheNode)
        :mNodeNameToFind(arNodeToExpandOrContract),
         mpSelectedSumNode(0),
         mActionTaken(ENodeNotFound),
         mJustFindTheNode(aJustFindTheNode)
{
}

CSumNodeExpanderVisitor::~CSumNodeExpanderVisitor()
{
}

void CSumNodeExpanderVisitor::Accept(CSumNode& arSumNode)
{
        NodeType Type = arSumNode.GetType();

        if (mNodeNameToFind == GetSumNodeUniqueName(arSumNode))
        {
                mpSelectedSumNode = &arSumNode;
                mActionTaken = ENodeFound;

                if (mJustFindTheNode == false && Type != EValueNode)
                {
                        if (arSumNode.IsNodeCollapsed())
                        {
                                arSumNode.ExpandNode();
                                mActionTaken = ENodeExpanded;
                        }
                        else
                        {
                                arSumNode.CollapseNode();
                                mActionTaken = ENodeContracted;
                        }
                }
        }
        else
        {
                if (Type != EValueNode && !arSumNode.IsLeaf())
                {
                        Accept(arSumNode.GetLeft());
                        Accept(arSumNode.GetRight());
                }
        }
}


bool CSumNodeExpanderVisitor::SelectedNodeWasFound() const
{
        return (mpSelectedSumNode !=0 && mActionTaken != ENodeNotFound);
}

CSumNode& CSumNodeExpanderVisitor::GetSelectedNode() const
{
        assert(SelectedNodeWasFound());
        return *mpSelectedSumNode;
}

bool CSumNodeExpanderVisitor::WasNodeContracted() const
{
        return mActionTaken == ENodeContracted;
}
