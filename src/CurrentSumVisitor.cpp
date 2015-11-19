#include "CurrentSumVisitor.h"
#include <assert.h>
#include "wx/log.h"
#include "SumNode.h"

CCurrentSumVisitor::CCurrentSumVisitor()
{
}

CCurrentSumVisitor::~CCurrentSumVisitor()
{
}

void CCurrentSumVisitor::Accept(CSumNode& arSumNode)
{
        size_t StartIdx = mSum.Length();

        if (arSumNode.GetBrackets())
        {
                mSum += _T("(");
        }
        if (!arSumNode.IsLeaf())
        {
                Accept(arSumNode.GetLeft());
        }

        wxString Value = GetNodeText(arSumNode);

        mSum += Value;

        if (!arSumNode.IsLeaf())
        {
                Accept(arSumNode.GetRight());
        }

        if (arSumNode.GetBrackets())
        {
                mSum += _T(")");
        }

        if (!arSumNode.IsLeaf() && arSumNode.GetPercentage())
        {
                mSum += _T("\%");
        }

        size_t EndIdx = mSum.Length();

        CStartEndPair StartEnd;
        StartEnd.first = StartIdx;
        StartEnd.second = EndIdx;
        mNodeBounds.insert(std::make_pair(&arSumNode, StartEnd));
        //wxLogError(_T("Adding %s with %d %d"), Value.c_str(), StartIdx, EndIdx)
}

const wxString& CCurrentSumVisitor::GetSum() const
{
        return mSum;
}

bool CCurrentSumVisitor::GetNodeLimits(CSumNode& arSumNode, size_t& arStart, size_t& arEnd) const
{
        bool Result = false;
        CSumBoundsMapIterator It = mNodeBounds.find(&arSumNode);

        if (It != mNodeBounds.end())
        {
                Result = true;
                arStart = It->second.first;
                arEnd = It->second.second;
        }

        return Result;
}

void CCurrentSumVisitor::Clear()
{
        mNodeBounds.clear();
        mSum.Clear();
}
