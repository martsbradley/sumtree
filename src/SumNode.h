#ifndef SUMNODE_H
#define SUMNODE_H
#include <wx/string.h>
#include <stdexcept>

class CSumNodeVisitor;

enum NodeType
{
    EValueNode,
    EAdditionNode,
    ESubtractionNode,
    EMultiplicationNode,
    EDivisionNode,
    EAddPercent,    
    ESubtractPercent,
    EMultiplyPercent,
    EDividePercent, 
    EExponential
};


class CSumNode
{
public:
        //  Value node
        CSumNode(double aValue);

        CSumNode(NodeType aType, CSumNode* apLeftNode, CSumNode* apRightNode);
        ~CSumNode();

        void Visit(CSumNodeVisitor& arVisitor);


        double GetValue() const;
        NodeType GetType() const;

        void SetBrackets(bool aHasBrackets);
        bool GetBrackets() const;

        void SetPercentage(bool aPercentage);
        bool GetPercentage() const;

        //  Only if IsLeaf is false then the GetLeft & GetRight will fail
        bool IsLeaf() const;
        CSumNode& GetLeft() const;
        CSumNode& GetRight() const;

        void CollapseNode();
        void ExpandNode();

        bool IsNodeCollapsed() const;

        double GetNodeValue() const;

private:
        NodeType mType;
        double mValue;
        CSumNode* mpLeftNode;
        CSumNode* mpRightNode;
        bool mIsCollapsed;
        bool mHasBrackets;
        bool mIsPercentage;
};

CSumNode* CreateNewNode(NodeType aType, CSumNode* apLeftNode, CSumNode* apRightNode);
CSumNode* CreateNewNode(double aValue);
CSumNode* HasBrackets(CSumNode* apNode);
CSumNode* IsPercentage(CSumNode* apNode);


wxString GetName(NodeType aType);
wxString GetSymbol(NodeType aType);

wxString GetSumNodeUniqueName(const CSumNode& arSumNode);

wxString GetNodeText(const CSumNode& arSumNode);

class CDivideByZeroException: public std::exception
{
};

#endif // SUMNODE_H
