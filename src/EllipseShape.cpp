#include "EllipseShape.h"
#include "wstringconvert.h"
#include <wx/log.h>

// should contain four numbers, topleft and bottomright coordinates
CEllipseShape::CEllipseShape(const std::vector<wxPoint>& arCoords)
        :CShapeLocation(),
         mOriginAdjustX(0),
         mOriginAdjustY(0),
         mHalfMajorAxis(0),
         mHalfMinorAxis(0)
{
        if (arCoords.size() != 2)
        {
                wxString Message = _T("Need two coordinates, the top left, then the width and height");
                wxLogError(Message);
        }

        wxPoint TopLeft = arCoords[0];

        mHalfMajorAxis = arCoords[1].x/2;//  Width
        mHalfMinorAxis = arCoords[1].y/2;//  Height
        //wxLogError(_T("Cons TopLeft.y is %d"), TopLeft.y);
        //wxLogError(_T("Cons arCoords[1].y is %d"), arCoords[1].y);

        mOriginAdjustX = 0.0 - (TopLeft.x + mHalfMajorAxis);
        mOriginAdjustY = 0.0 - (TopLeft.y + mHalfMinorAxis);
}

bool CEllipseShape::Contains(const wxPoint& arPoint) const
{
        double X = arPoint.x;
        double Y = arPoint.y;
        X += mOriginAdjustX;
        Y += mOriginAdjustY;

        double Result = 0.0;
        
        if (mHalfMinorAxis > 0 && mHalfMajorAxis > 0)
        {
                Result = (X*X)/(mHalfMajorAxis*mHalfMajorAxis);
                Result += (Y*Y)/(mHalfMinorAxis*mHalfMinorAxis);
        }

        return Result < 1.0;
}
