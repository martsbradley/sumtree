#ifndef CIRCLE_SHAPE_H
#define CIRCLE_SHAPE_H
#include "ShapeLocation.h"
#include <wx/gdicmn.h>
#include <vector>

class CCircleShape: public CShapeLocation
{
public:
        CCircleShape(const std::vector<wxPoint>& arCoords); 

        bool Contains(const wxPoint& arPoint) const;

private:
        wxPoint mCenter;
        int mRadius;
};
#endif // CIRCLE_SHAPE_H
