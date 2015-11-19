#ifndef BOX_SHAPE_H
#define BOX_SHAPE_H
#include "ShapeLocation.h"
#include <vector>

#include <wx/gdicmn.h>
class CBoxShape: public CShapeLocation
{
public:
        CBoxShape(const std::vector<wxPoint>& arCoords);

        bool Contains(const wxPoint& arPoint) const;

private:
        wxPoint mTopLeft;
        wxPoint mBottomRight;
};
#endif // BOX_SHAPE_H
