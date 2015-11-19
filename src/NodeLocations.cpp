#include "NodeLocations.h"
#include "BoxShape.h"
#include "CircleShape.h"
#include "EllipseShape.h"
#include <stdexcept>


static CShapeLocation* GetShape(CNodeLocations::EObjectShapes aShape, 
                                const std::vector<wxPoint>& arCoords)
{
        CShapeLocation* pResult = 0;

        if (aShape == CNodeLocations::ESquare)
        {
                pResult = new CBoxShape(arCoords);
        }
        else if (aShape == CNodeLocations::ECircle)
        {
                pResult = new CCircleShape(arCoords);
        }
        else if (aShape == CNodeLocations::EEllipse)
        {
                pResult = new CEllipseShape(arCoords);
        }

        return pResult;
}



const void* CNodeLocations::GetObjectAtLocation(const wxPoint& arPoint) const
{
        const void* pObject = 0;

        for (CNodeLocationsIterator It = mNodeLocations.begin(); 
             It != mNodeLocations.end();
             ++It)
        {
                if (It->second->Contains(arPoint))
                {
                        pObject = It->first;
                        break;
                }
        }

        return pObject;
}

void CNodeLocations::InsertNode(const void* apObject,
                                EObjectShapes aObjectShape,
                                const std::vector<wxPoint>& arCoords)
{
        CShapeLocation* pShape = GetShape(aObjectShape, arCoords);

        if (pShape)
        {
                CShapeLocationPtr pBox(pShape);
                mNodeLocations.insert(std::make_pair(apObject, pBox));
        }
}

void CNodeLocations::Clear()
{
        mNodeLocations.clear();
}
