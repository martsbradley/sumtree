#ifndef ELLIPSE_SHAPE_H
#define ELLIPSE_SHAPE_H
#include "ShapeLocation.h"
#include <vector>
#include <wx/gdicmn.h>


// On construction I'll move the ellipse so that it is center at the origin.
// I'll store the adjustment needed to do that in the member mOriginAdjust
// The values mHalfMajorAxis and mHalfMinorAxis will store the 1/2 of the major
// and minor axis respectivly.
// So when the point comes in on the Contains method, 
// the point is adjusted by mOriginAdjust to be (X', Y').  After that the 
// points (X', Y') will be applied to the following and if its returns true
// then the point lies within this ellipse.


//  X'^2        Y'^2
//  ___    *    ____     < 1
//  mA^2        mB^2

class CEllipseShape: public CShapeLocation
{
public:
        CEllipseShape(const std::vector<wxPoint>& arCoords); 

        bool Contains(const wxPoint& arPoint) const;

private:
        double mOriginAdjustX;
        double mOriginAdjustY;

        double mHalfMajorAxis;
        double mHalfMinorAxis;
};
#endif // ELLIPSE_SHAPE_H
