#ifndef DOTSUMNODE_EXPANDERVISITOR_H
#define DOTSUMNODE_EXPANDERVISITOR_H
#include "SumNodeVisitor.h"
#include "wx/string.h"

class CSumNodeExpanderVisitor : public CSumNodeVisitor
{
public:
        CSumNodeExpanderVisitor(const wxString& arNodeToExpandOrContract,
                                bool aJustFindTheNode);

        virtual ~CSumNodeExpanderVisitor();
        virtual void Accept(CSumNode& arSumNode);

        bool SelectedNodeWasFound() const;

        CSumNode& GetSelectedNode() const;

        //  True if the node was found and changed to be contracted.
        bool WasNodeContracted() const;
private:
        enum EActionTaken
        {
                ENodeNotFound,
                ENodeFound,
                ENodeContracted,
                ENodeExpanded
        };

        wxString mNodeNameToFind;
        CSumNode* mpSelectedSumNode;
        EActionTaken mActionTaken;

        //  If this is true then just find the node and don't change anything
        bool mJustFindTheNode;
};
#endif // DOTSUMNODE_EXPANDERVISITOR_H
