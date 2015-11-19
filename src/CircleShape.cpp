#include "CircleShape.h"
#include "wstringconvert.h"
#include <wx/log.h>

// should contain four numbers, topleft and bottomright coordinates
CCircleShape::CCircleShape(const std::vector<wxPoint>& arCoords)
        :CShapeLocation()
{
        if (arCoords.size() != 2)
        {
                wxString Message = _T("Need two coordinates, the center and the radius ")
                                   _T("(held in the x value of the second point");
                wxLogError(Message);
        }

        mCenter = arCoords[0];
        mRadius = arCoords[1].x;

        //wxString Message;
        //Message << "(" << Values[0] << "," << Values[1] << ") radius " << mRadius;
        //::wxLogMessage(Message);
}

bool CCircleShape::Contains(const wxPoint& arPoint) const
{
        int X = arPoint.x;
        int Y = arPoint.y;

        int CenterX = mCenter.x;
        int CenterY = mCenter.y;

        int Value = ((CenterX - X) * (CenterX - X)) +
                     ((CenterY - Y) * (CenterY - Y));
        
        /*wxString Message;
        Message << "Value = " << Value << " Radius " << mRadius*mRadius;
        ::wxLogMessage(Message);*/

        return (Value <= (mRadius*mRadius));
}
