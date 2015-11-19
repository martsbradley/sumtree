#include "SumNode.h"
#include "SumNodeVisitor.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <wx/tokenzr.h>



struct OperatorTypes{
    NodeType Type;
    wxChar Name[20];
    wxChar Symbol[2];
};

struct OperatorTypes Names[] = {{ EAdditionNode,      _T("Add"),             _T("+")},
                                { ESubtractionNode,   _T("Subtract"),        _T("-")},
                                { EMultiplicationNode,_T("Multiply"),        _T("*")},
                                { EDivisionNode,      _T("Divide"),          _T("/")},
                                { EAddPercent,        _T("AddPercent"),      _T("+")},
                                { ESubtractPercent,   _T("SubtractPercent"), _T("-")},
                                { EMultiplyPercent,   _T("MultiplyPercent"), _T("*")},
                                { EDividePercent,     _T("DividePercent"),   _T("/")},
                                { EExponential,       _T("Exponential"),     _T("^")}, 
                                { EValueNode,         _T("Value"),           _T("") }};

static const OperatorTypes* GetDetails(NodeType aType)
{
        const OperatorTypes* pResult = 0;

        for (int Idx = 0; Idx < 10; ++Idx)
        {
                if (Names[Idx].Type == aType)
                        pResult = &Names[Idx];
        }

        return pResult;
}

wxString GetName(NodeType aType)
{
    wxString Result;
    const OperatorTypes* pDetail = GetDetails(aType);

    if (pDetail)
            Result = &pDetail->Name[0];

    return Result;
}

wxString GetSymbol(NodeType aType)
{
    wxString Result;

    const OperatorTypes* pDetail = GetDetails(aType);

    if (pDetail)
            Result = pDetail->Symbol;
    return Result;
}

double CSumNode::GetNodeValue() const
{
        double Result = 0;

        if (mType == EValueNode)
        {
                Result = mValue;
        }
        if (mType == EAdditionNode)
        {
                Result = mpLeftNode->GetNodeValue() + mpRightNode->GetNodeValue();
        }
        if (mType == ESubtractionNode)
        {
                Result = mpLeftNode->GetNodeValue() - mpRightNode->GetNodeValue();
        }
        if (mType == EMultiplicationNode)
        {
                Result = mpLeftNode->GetNodeValue() * mpRightNode->GetNodeValue();
        }
        if (mType == EDivisionNode)
        {
                if (mpRightNode->GetNodeValue() == 0)
                {
                        CDivideByZeroException CannotDivideByZero;
                        throw CannotDivideByZero;
                }

                Result = mpLeftNode->GetNodeValue() / mpRightNode->GetNodeValue();
        }
        if (mType == EAddPercent)
        {
                double Left = mpLeftNode->GetNodeValue();
                double Right =  mpRightNode->GetNodeValue();
                Result = Left + (Left * Right/100);
        }
        if (mType == ESubtractPercent)
        {
                double Left = mpLeftNode->GetNodeValue();
                double Right =  mpRightNode->GetNodeValue();
                Result = Left - (Left * Right/100);
        }

        if (mType == EMultiplyPercent)
        {
                double Left = mpLeftNode->GetNodeValue();
                double Right =  mpRightNode->GetNodeValue();
                Result = Left * (Left * Right/100);
        }

        if (mType == EDividePercent)
        {
                double Left = mpLeftNode->GetNodeValue();
                double Right =  mpRightNode->GetNodeValue();
                double Divisior = (Left * Right/100);;

                if (Divisior == 0)
                {
                        CDivideByZeroException CannotDivideByZero;
                        throw CannotDivideByZero;
                }

                Result = Left / Divisior;
        }

        if (mType == EExponential)
        {
                double Left = mpLeftNode->GetNodeValue();
                double Right =  mpRightNode->GetNodeValue();
                Result = pow(Left,Right);
        }
        return Result;
}


char SumBuffer[1024];
int SumPtr=0;
int PreviousValueOfSumPtr=0;

CSumNode* CreateNewNode(NodeType aType, CSumNode* apLeftNode, CSumNode* apRightNode)
{
        CSumNode* pSumNode = new CSumNode(aType, apLeftNode, apRightNode);
        return pSumNode;
}
CSumNode* CreateNewNode(double aValue)
{
        CSumNode* pSumNode = new CSumNode(aValue);
        return pSumNode;
}

CSumNode* HasBrackets(CSumNode* apNode)
{
        apNode->SetBrackets(true);
        return apNode;
}

CSumNode* IsPercentage(CSumNode* apNode)
{
        apNode->SetPercentage(true);
        return apNode;
}

CSumNode::CSumNode(double aValue)
        :mType(EValueNode),
         mValue(aValue),
         mpLeftNode(0),
         mpRightNode(0),
         mIsCollapsed(false),
         mHasBrackets(false),
         mIsPercentage(false)
{
}

CSumNode::CSumNode(NodeType aType,
                   CSumNode* apLeftNode,
                   CSumNode* apRightNode)
        :mType(aType),
         mValue(0),
         mpLeftNode(apLeftNode),
         mpRightNode(apRightNode),
         mIsCollapsed(false),
         mHasBrackets(false),
         mIsPercentage(false)
{
}

CSumNode::~CSumNode()
{
        delete mpLeftNode;
        delete mpRightNode;
}
void CSumNode::Visit(CSumNodeVisitor& arVisitor)
{
    arVisitor.Accept(*this);
}

double CSumNode::GetValue() const
{
        return mValue;
}

NodeType CSumNode::GetType() const
{
        return mType;
}

void CSumNode::SetBrackets(bool aHasBrackets)
{
        mHasBrackets = aHasBrackets;
}

bool CSumNode::GetBrackets() const
{
        return mHasBrackets;
}

void CSumNode::SetPercentage(bool aPercentage)
{
        mIsPercentage = aPercentage;
}

bool CSumNode::GetPercentage() const
{
        return mIsPercentage;
}

//  Only if IsLeaf is false then the GetLeft & GetRight will fail
bool CSumNode::IsLeaf() const
{
        return (mIsCollapsed ||  mpLeftNode == 0 || mpRightNode == 0);
}

CSumNode& CSumNode::GetLeft() const
{
        assert(!IsLeaf() && mpLeftNode );
        return *mpLeftNode;
}

CSumNode& CSumNode::GetRight() const
{
        assert(!IsLeaf() && mpRightNode);
        return *mpRightNode;
}

// Now make this node look like a value node if it is not one already.
void CSumNode::CollapseNode()
{
        mIsCollapsed = true;
        mValue = GetNodeValue();
}

void CSumNode::ExpandNode()
{
        mIsCollapsed = false;
}

bool CSumNode::IsNodeCollapsed() const
{
        return mIsCollapsed;
}


wxString GetSumNodeUniqueName(const CSumNode& arSumNode)
{
         wxString Result = GetName(arSumNode.GetType());
         Result << (int)&arSumNode;
         return Result;
}

static void FormatValue(wxString& arValue)
{
        wxStringTokenizer tkz(arValue, wxT("."));

        wxString WholePart = tkz.GetNextToken();

        if (tkz.HasMoreTokens())
        {
                wxString DecimalPart = tkz.GetNextToken();

                int Length = DecimalPart.Length();


                while (Length > 0 && DecimalPart[Length-1] == _T('0'))
                {
                        DecimalPart = DecimalPart(0, Length-1);
                        --Length;
                }


                arValue= WholePart;

                if (DecimalPart.Length() > 0)
                {
                        arValue += + _T(".") + DecimalPart;
                }
        }
}

wxString GetNodeText(const CSumNode& arSumNode)
{
        wxString Value;

        NodeType Type = arSumNode.GetType();

        if (Type == EValueNode || arSumNode.IsNodeCollapsed())
        {
                double NodeValue = arSumNode.GetValue();

                Value = wxString::Format(_T("%.9f"), NodeValue);

                FormatValue(Value);
        }
        else
        {
                Value << GetSymbol(Type);
        }


        if ((Type == EValueNode || arSumNode.IsNodeCollapsed()) && arSumNode.GetPercentage())
                Value += _T("\%");


        return Value;
}
