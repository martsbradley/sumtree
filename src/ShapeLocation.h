#ifndef SHAPELOCATION_H
#define SHAPELOCATION_H

class wxPoint;

class CShapeLocation
{
public:
        virtual ~CShapeLocation() {};

        virtual bool Contains(const wxPoint& arPoint) const = 0;
};
#endif // SHAPELOCATION_H
