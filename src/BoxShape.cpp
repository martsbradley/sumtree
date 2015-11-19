#include "BoxShape.h"
#include "wstringconvert.h"
#include <wx/log.h>

// should contain four numbers, topleft and bottomright coordinates
CBoxShape::CBoxShape(const std::vector<wxPoint>& arCoords)
        :CShapeLocation()
{
        size_t ArraySize = arCoords.size();

        if (ArraySize != 2)
        {
                wxString message = _T("Need two coordinates");
                //throw std::invalid_argument(message);
                wxLogError(message);
        }

        mTopLeft     = arCoords[0];
        mBottomRight = arCoords[1];
}

/*  X increases further right
    Y increases further right
*/
bool CBoxShape::Contains(const wxPoint& arPoint) const
{
        int X = arPoint.x;
        int Y = arPoint.y;

        return X >= mTopLeft.x     &&
               X <= mBottomRight.x &&
               Y >= mTopLeft.y     && 
               Y <= mBottomRight.y;
}
