#include "Layout.h"
#include "NodeLocations.h"
#include <assert.h>
#include <wx/dc.h>
#include <wx/log.h>

int RankHeight = 55;      // Height of a rank.
int RankSeparation = 40;  // Distance between ranks.
int NodeSeparation = 40;  // Minimum distance between nodes.
int BorderWidth = 15;
extern int TextHeight;

////////////////////////////////////////////////////////////////////////////////
//   CLayout
////////////////////////////////////////////////////////////////////////////////
CLayout::CLayout(EOrientation aOrientation)
        :mOrientation(aOrientation)
{
        mRanks.reserve(100);
}

CNodeInfo* CLayout::AddNode(CNodeInfo& arNodeInfo, size_t aRank)
{
        //  aRank could be a new rank so <= size()
        size_t RankCount = mRanks.size();
        assert(aRank <= RankCount);

        if(aRank == RankCount)
        {
                CRank Rank(aRank);
                mRanks.push_back(Rank);
        }

        assert(aRank < mRanks.size());

        return mRanks[aRank].AppendNode(arNodeInfo, *this);
}

// Move the parents between there children
void CLayout::Layout()
{
        // Start at the rank second from the bottom
        // Start at the last node in the rank.
        // Ask what are the Center of the left and right children
        // Move the node to the center of its children
        //
        // After this is done for each rank go back and move any children
        // that are too far from their parents under them
        //

        if (mOrientation == ETreeUp)
        {
                size_t MaxRank = mRanks.size()- 1;

                for (size_t Idx = 0; Idx < mRanks.size(); ++Idx)
                {
                        mRanks[Idx].SetRank(MaxRank - Idx);
                }
        }

        bool Result = true;
        int Times = 0;

        while (Result && Times++ < 10000)
        {
                MoveParents();
                Result = MoveChildren();
        }
}
void CLayout::MoveParents()
{
        size_t RankCount = mRanks.size();
        size_t RankId = RankCount-1;

        while (RankId > 0)
        {
                --RankId;
                mRanks[RankId].MoveNodesOverChildren();
        }
}
bool CLayout::MoveChildren()
{
        bool ChildMoved = false;
        //  Now move any children right are too far left
        //  from their parents.
        for (size_t Idx = 0; Idx < mRanks.size(); ++Idx)
        {
                CRank& rRank = mRanks[Idx];
                int Distance = 0;

                CNodeInfo* pParent = rRank.WhichParentHasChildrenWhichNeedMoved(Distance);

                if (pParent)
                {
                        mRanks[Idx+1].MoveNodeBy(pParent->GetLeftChild(), Distance);
                        ChildMoved = true;
                        //break;
                }
        }
        return ChildMoved;
}

CNodeInfo* CLayout::GetLastNodeOnRank(size_t aRankIdx)
{
        assert(aRankIdx < mRanks.size());

        CRank& rRank = mRanks[aRankIdx];

        CNodeInfo* pNode = rRank.GetLastNode();
        return pNode;
}

void CLayout::Draw(wxDC& arDC, CNodeLocations& arNodeLocation) const 
{
        size_t RankCount = mRanks.size();
        for (size_t RankId = 0; RankId < RankCount; ++RankId)
        {
                mRanks[RankId].Draw(arDC, arNodeLocation, mOrientation);
        }

}


////////////////////////////////////////////////////////////////////////////////
//   CRank
////////////////////////////////////////////////////////////////////////////////



CRank::CRank()
        :mRankIdx(0)
{
        SetBaseLineFromRank(mRankIdx);
        mNodes.reserve(100);
}

CRank::CRank(size_t aRankIdx)
        :mRankIdx(aRankIdx)
{
        SetBaseLineFromRank(mRankIdx);

        mNodes.reserve(100);
}

CRank::CRank(const CRank& arRank)
        :mRankIdx(arRank.mRankIdx)
{
        SetBaseLineFromRank(mRankIdx);

        mNodes.reserve(100);
        for (size_t Idx = 0; Idx < arRank.mNodes.size(); ++Idx)
        {
                mNodes[Idx] = arRank.mNodes[Idx];
        }
}

void CRank::SetBaseLineFromRank(size_t aRankIdx)
{
        mRankBaseLine = ((aRankIdx)* RankHeight) + (aRankIdx * RankSeparation);
}
void CRank::SetRank(size_t aRankIdx)
{
        mRankIdx = aRankIdx;
        SetBaseLineFromRank(aRankIdx);

        for (size_t Idx = 0; Idx <mNodes.size(); ++Idx)
        {
                mNodes[Idx].SetRankBaseLine(mRankBaseLine);
        }
}

// Start at the last node.
// Ask for the positions of the children.
// Move this node to the middle of those points.
void CRank::MoveNodesOverChildren()
{
        size_t NodeCount = mNodes.size();

        for (size_t NodeIdx = 0; NodeIdx < NodeCount; ++NodeIdx)
        {
                CNodeInfo& rNode = mNodes[NodeIdx];

                if (rNode.IsParent())
                {
                        wxPoint LeftChild =  rNode.GetLeftChildsPosition();
                        wxPoint RightChild = rNode.GetRightChildsPosition();
                        wxPoint ThisNode =   rNode.GetPosition();

                        int NewPosition = (LeftChild.x + RightChild.x)/2;
                        NewPosition =  NewPosition + (NewPosition % 2);

                        int CurrentPosition = ThisNode.x;
                        int Distance  = NewPosition - CurrentPosition;

                        MoveNodeBy(rNode, Distance);
                }
        }
}
// For each parent if it is not at the center point of its children then move its children to be
CNodeInfo* CRank::WhichParentHasChildrenWhichNeedMoved(int& arDistance)
{
        size_t NodeCount = mNodes.size();
        CNodeInfo* pParentWithUnBalancedChildren = 0;

        for (size_t NodeIdx = 0; NodeIdx < NodeCount; ++NodeIdx)
        {
                CNodeInfo& rNode = mNodes[NodeIdx];

                if (rNode.IsParent())
                {
                        wxPoint LeftChild =  rNode.GetLeftChildsPosition();
                        wxPoint RightChild = rNode.GetRightChildsPosition();
                        wxPoint ThisNode =   rNode.GetPosition();

                        int CurrentChildMidPoint = (LeftChild.x + RightChild.x)/2;
                        CurrentChildMidPoint =  CurrentChildMidPoint + (CurrentChildMidPoint % 2);

                        int ChildsMidPoint = ThisNode.x;

                        if (abs(ChildsMidPoint - CurrentChildMidPoint) > 5)
                        {
                                //wxLogError(_T("Node %s needs seen to"),rNode.GetText().c_str());
                                arDistance =  ChildsMidPoint - CurrentChildMidPoint;
                                pParentWithUnBalancedChildren  = &rNode;
                                break;
                        }
                }
        }
        return pParentWithUnBalancedChildren;
}


CNodeInfo* CRank::AppendNode(const CNodeInfo& arNode, CLayout& /*arLayout*/)
{
        CNodeInfo Node(arNode);
        int XPosition = 0;

        if (!mNodes.empty())
        {
                const CNodeInfo& rRightMostNode = mNodes.back();
                XPosition = rRightMostNode.GetXPosition() + rRightMostNode.GetWidth() + NodeSeparation;
                //wxLogError(_T("Setting X for to %d"),  XPosition);
        }

        Node.SetXPosition(XPosition);
        Node.SetRankBaseLine(mRankBaseLine);

        assert(mNodes.capacity() > 0);
        mNodes.push_back(Node);

        return &mNodes.back();
}

// Attempt to move the given node.
// Ensure that we don't move the node backwards over the earlier node.
// But ensure that the node remains on the page 
void CRank::MoveNodeBy(const CNodeInfo& arNode, int aDistance)
{
        for (size_t NodeIdx = 0;NodeIdx < mNodes.size(); ++NodeIdx)
        {
                if (&arNode == &mNodes[NodeIdx])
                {
                        size_t IdxBack = mNodes.size();

                        while(IdxBack > NodeIdx)
                        {
                                --IdxBack;
                                CNodeInfo& rNodeToMove = mNodes[IdxBack];
                                int OriginalPosition = rNodeToMove.GetXPosition();
                                int NewPosition = OriginalPosition + aDistance;

                                bool ValidMove = true;

                                if (IdxBack > 0 && aDistance < 0)
                                {
                                        //  There are nodes to the left of this one,
                                        //  ensure that we are not trying to move the
                                        //  node into the area of the other node.
                                        CNodeInfo& rNodeOnLeft = mNodes[IdxBack-1];

                                        int MinXPosition = rNodeOnLeft.GetXPosition() + rNodeOnLeft.GetWidth() + NodeSeparation;

                                        if (NewPosition < MinXPosition)
                                                ValidMove = false;
                                }

                                if (ValidMove)
                                        rNodeToMove.SetXPosition(NewPosition);
                        }

                        break;
                }
        }
}

CNodeInfo* CRank::GetLastNode()
{
        assert(mNodes.size() > 0);
        return &mNodes[mNodes.size()-1];
}

void CRank::Draw(wxDC& arDC, CNodeLocations& arNodeLocation, EOrientation aOrientation) const
{
        for (size_t NodeId = 0; NodeId < mNodes.size(); ++NodeId)
        {
                mNodes[NodeId].Draw(arDC, arNodeLocation, aOrientation);
        }
}
////////////////////////////////////////////////////////////////////////////////
//   CNodeInfo
////////////////////////////////////////////////////////////////////////////////
CNodeInfo::CNodeInfo()
       :mpParent(0),
        mpLeftChild(0),
        mpRightChild(0),
        mWidth(0),
        mXPosition(0),
        mRankBaseLine(0),
        mText(_T("")),
        mNodeType(EValueNode),
        mIsNodeCollapsed(false),
        mpObject(0)
{
}


CNodeInfo::CNodeInfo(const wxString& arText,
                     int aWidth,
                     NodeType aNodeType,
                     bool aIsNodeCollapsed,
                     const void* apObject)
       :mpParent(0),
        mpLeftChild(0),
        mpRightChild(0),
        mWidth(aWidth),
        mXPosition(0),
        mRankBaseLine(0),
        mText(arText),
        mNodeType(aNodeType),
        mIsNodeCollapsed(aIsNodeCollapsed),
        mpObject(apObject)
{
        mWidth += BorderWidth*2;
        /*if (aNodeType != EValueNode)
        {
        }*/
}

CNodeInfo::CNodeInfo(const CNodeInfo& arNodeInfo)
       :mpParent(arNodeInfo.mpParent),
        mpLeftChild(arNodeInfo.mpLeftChild),
        mpRightChild(arNodeInfo.mpRightChild),
        mWidth(arNodeInfo.mWidth),
        mXPosition(arNodeInfo.mXPosition),
        mRankBaseLine(arNodeInfo.mRankBaseLine),
        mText(arNodeInfo.mText),
        mNodeType(arNodeInfo.mNodeType),
        mIsNodeCollapsed(arNodeInfo.mIsNodeCollapsed),
        mpObject(arNodeInfo.mpObject) 
{
}

CNodeInfo& CNodeInfo::operator=(const CNodeInfo& arNode)
{
        mpParent      = arNode.mpParent;
        mpLeftChild   = arNode.mpLeftChild;
        mpRightChild  = arNode.mpRightChild;
        mWidth        = arNode.mWidth;
        mXPosition    = arNode.mXPosition;
        mRankBaseLine = arNode.mRankBaseLine;
        mText         = arNode.mText;
        mNodeType     = arNode.mNodeType;
        mIsNodeCollapsed  = arNode.mIsNodeCollapsed;
        mpObject     =  arNode.mpObject;
        return *this;
}


void CNodeInfo::SetParent(CNodeInfo* apParent)
{
        assert(mpParent == 0);
        assert(apParent != 0);
        mpParent = apParent;
        //wxLogError(_T("Parent of %s [%p] is %p"), mText.c_str(), this, mpParent);
}
void CNodeInfo::SetLeftChild(CNodeInfo* apChild)
{
        assert(mpLeftChild == 0);
        mpLeftChild = apChild;
        //wxLogError(_T("LeftChild of %s [%p] is %p"), mText.c_str(), this, mpLeftChild);
}
void CNodeInfo::SetRightChild(CNodeInfo* apChild)
{
        assert(mpRightChild == 0);
        mpRightChild = apChild;
        //wxLogError(_T("RightChild of %s [%p] is %p"), mText.c_str(), this, mpRightChild);
}
int CNodeInfo::GetXPosition() const
{
        return mXPosition;
}

void CNodeInfo::SetXPosition(int aXPosition)
{
        mXPosition = aXPosition;
}

// Position of the center of this node.
wxPoint CNodeInfo::GetPosition() const
{
        return wxPoint(mXPosition + (mWidth/2), mRankBaseLine + (RankHeight/2));
}

wxPoint CNodeInfo::GetLeftChildsPosition()
{
        assert(IsParent());
        return mpLeftChild->GetPosition();
}

wxPoint CNodeInfo::GetRightChildsPosition()
{
        assert(IsParent());
        return mpRightChild->GetPosition();
}

void CNodeInfo::SetRankBaseLine(int aRankBaseLine)
{
        mRankBaseLine = aRankBaseLine;
}

int CNodeInfo::GetRankBaseLine() const
{
        return mRankBaseLine;
}

int CNodeInfo::GetDistanceToParent()
{
        return 0;
}

bool CNodeInfo::IsParent() const
{
        return (mpLeftChild != 0 && mpRightChild != 0);
}

int CNodeInfo::GetWidth() const
{
        return mWidth;
}

const CNodeInfo& CNodeInfo::GetLeftChild() const
{
        assert(IsParent());
        return *mpLeftChild;
}

const wxString& CNodeInfo::GetText() const
{
        return mText;
}

static int GetTextHeight(wxDC& arDC, const wxString& arString)
{
        int Width;
        int Height;
        arDC.GetTextExtent(arString, &Width, &Height);
        return Height;
}

void CNodeInfo::DrawGridLines(wxDC& arDC) const
{
        arDC.SetPen(*wxGREEN);
        arDC.DrawLine(0, mRankBaseLine, mXPosition, mRankBaseLine);
        arDC.SetPen(*wxBLACK);
        arDC.DrawLine(0, mRankBaseLine+RankHeight, mXPosition, mRankBaseLine+RankHeight);
}

void CNodeInfo::DrawParentArrows(wxDC& arDC, EOrientation aOrientation) const
{
        arDC.SetPen(*wxBLACK);
        wxPoint ParentPostion = mpParent->GetPosition();

        if (aOrientation == ETreeUp)
        {
                ParentPostion.y -= RankHeight/2;
        }
        else
        {
                ParentPostion.y += -1 + RankHeight/2;
        }

        arDC.DrawLine(GetPosition(), ParentPostion);
}

void CNodeInfo::Draw(wxDC& arDC, CNodeLocations& arNodeLocation, EOrientation aOrientation) const
{
        /*if (false)
                DrawGridLines(arDC);*/
        wxColor Lime (156,254,132);
        wxColor Blue1(192,178,252);
        wxColor Blue2(188,222,236);
        wxColor Yellow(236,254,156);

        if (mpParent)
        {
                DrawParentArrows(arDC, aOrientation);
        }

        if (mNodeType == EValueNode)
        {
                arDC.SetBrush(*wxRED);
                arDC.SetPen(*wxRED);
                arDC.SetBrush(Lime);
                arDC.SetPen(Lime);


                arDC.DrawEllipse(mXPosition,
                                 mRankBaseLine,
                                 mWidth,
                                 RankHeight);
                //wxLogError("DrawEllipse %d %d %d %d", mXPosition,
                                 //mRankBaseLine,
                                 //mWidth,
                                 //RankHeight);
                std::vector<wxPoint> EllipseCoords;
                EllipseCoords.push_back(wxPoint(mXPosition, mRankBaseLine));
                EllipseCoords.push_back(wxPoint(mWidth, RankHeight));

                arNodeLocation.InsertNode(mpObject, CNodeLocations::EEllipse, EllipseCoords);
        }
        else
        {
                arDC.SetBrush(Blue2);
                arDC.SetPen(*wxBLACK);
                if (mIsNodeCollapsed)
                        arDC.SetBrush(*wxGREEN);

                arDC.DrawRectangle(mXPosition,
                                   mRankBaseLine,
                                   mWidth,
                                   RankHeight);

                std::vector<wxPoint> SquareCoords;
                SquareCoords.push_back(wxPoint(mXPosition, mRankBaseLine));
                SquareCoords.push_back(wxPoint(mXPosition+mWidth, mRankBaseLine+RankHeight));

                arNodeLocation.InsertNode(mpObject, CNodeLocations::ESquare, SquareCoords);
        }
                
        arDC.SetPen(*wxWHITE);

        int TextYOffset = (RankHeight - GetTextHeight(arDC,mText))/2;

        arDC.DrawText(mText, 
                      mXPosition + BorderWidth, 
                      mRankBaseLine + TextYOffset);

}
