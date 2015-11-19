#ifndef CURRENT_SUMVISITOR_H
#define CURRENT_SUMVISITOR_H
#include "SumNodeVisitor.h"
#include "wx/string.h"
#include <map>


class CCurrentSumVisitor : public CSumNodeVisitor
{
public:
        CCurrentSumVisitor();

        virtual ~CCurrentSumVisitor();
        virtual void Accept(CSumNode& arSumNode);

        const wxString& GetSum() const;

        bool GetNodeLimits(CSumNode& arSumNode, size_t& arStart, size_t& arEnd) const;

        void Clear();
private:
        wxString mSum;
        typedef std::pair<size_t,size_t> CStartEndPair;
        typedef std::map <const CSumNode*, CStartEndPair> CSumBoundsMap;
        typedef CSumBoundsMap::const_iterator CSumBoundsMapIterator;
        CSumBoundsMap mNodeBounds;
};
#endif // CURRENT_SUMVISITOR_H
