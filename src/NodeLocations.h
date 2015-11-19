#ifndef NODE_LOCATIONS
#define NODE_LOCATIONS
#include "CountedPtr.h"
#include "ShapeLocation.h"
#include <map>
#include <vector>

//  Class responsible for knowing the locations of elements in that image.
class CNodeLocations
{
public:
        enum EObjectShapes
        {
                ECircle,
                ESquare,
                EEllipse
        };

        void InsertNode(const void* apObject,
                         EObjectShapes aObjectShape,
                         const std::vector<wxPoint>& arCoords); 

        const void* GetObjectAtLocation(const wxPoint& arPoint) const;

        void Clear();

private:

        typedef CountedPtr<CShapeLocation> CShapeLocationPtr;

        // Map of pointer to object to the location of that object.
        typedef std::map<const void*, CShapeLocationPtr> CNodeLocationMap;
        typedef CNodeLocationMap::const_iterator CNodeLocationsIterator;

        CNodeLocationMap mNodeLocations;
};
#endif // NODE_LOCATIONS
