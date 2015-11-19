#ifndef LAYOUT_H
#define LAYOUT_H
#include <vector>
#include <wx/string.h>
#include "wx/gdicmn.h"
#include "SumNode.h"

class CNodeInfo;
class wxDC;
class CLayout;
class CNodeLocations;

enum EOrientation
{
        ETreeUp,
        ETreeDown
};



class CRank
{
public:
        CRank();
        CRank(size_t aRankIdx);
        CRank(const CRank& arRank);

        CNodeInfo* AppendNode(const CNodeInfo& arNode, CLayout& arLayout);

        void MoveNodeBy(const CNodeInfo& arNode, int aXPosition);

        void MoveNodesOverChildren();

        CNodeInfo* WhichParentHasChildrenWhichNeedMoved(int& arDistance);

        CNodeInfo* GetLastNode();

        void Draw(wxDC& arDC, CNodeLocations& arNodeLocation, EOrientation aOrientation) const;
        void SetRank(size_t aRank);
private:

        void SetBaseLineFromRank(size_t aRank);
        std::vector<CNodeInfo> mNodes;

        //  This is the y line base of the Rank.
        int mRankBaseLine;
        size_t mRankIdx;
};

class CLayout
{
public:
        CLayout(EOrientation aOrientation);

        CNodeInfo* AddNode(CNodeInfo& arNodeInfo, size_t aRank);

        void Layout();

        void Draw(wxDC& arDC, CNodeLocations& arNodeLocation) const;


        CNodeInfo* GetLastNodeOnRank(size_t aRankIdx);
private:
        void MoveParents();
        bool MoveChildren();
        std::vector<CRank> mRanks;
        EOrientation mOrientation;
};

class CNodeInfo
{
public:
        CNodeInfo();
        CNodeInfo(const wxString& arText,
                  int aWidth,
                  NodeType aNodeType,
                  bool aIsNodeCollapsed, 
                  const void* apObject);

        CNodeInfo(const CNodeInfo& arNodeInfo);

        CNodeInfo& operator=(const CNodeInfo& arNode);

        void SetLeftChild(CNodeInfo* apChild);
        void SetRightChild(CNodeInfo* apChild);

        // Sets the parent of this node.
        void SetParent(CNodeInfo* apParent);

        int GetXPosition() const;
        void SetXPosition(int aXPosition);

        wxPoint GetPosition() const;
        wxPoint GetLeftChildsPosition();
        wxPoint GetRightChildsPosition();
        void SetRankBaseLine(int aRankBaseLine);
        int GetRankBaseLine() const;

        int GetDistanceToParent();

        bool IsParent() const;
        int GetWidth() const;
        void Draw(wxDC& arDC, CNodeLocations& arNodeLocation, EOrientation aOrientation) const;
        const wxString& GetText() const;
        const CNodeInfo& GetLeftChild() const;
private:
        void DrawGridLines(wxDC& arDC) const;
        void DrawParentArrows(wxDC& arDC, EOrientation aOrientation) const;

        CNodeInfo* mpParent;
        CNodeInfo* mpLeftChild;
        CNodeInfo* mpRightChild;
        

        // These are always even numbers so that the middle can be found.
        int mWidth;

        //  Positional values.
        int mXPosition;     //  The left most point on this node.
        int mRankBaseLine;  //  The base line of this node.
        wxString mText;
        NodeType mNodeType;
        bool mIsNodeCollapsed;

        const void *mpObject; //  The object that this node represents.  void as its just used as an indentifier.
};
#endif // LAYOUT_H
