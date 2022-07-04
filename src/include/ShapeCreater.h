#ifndef GEOPAD_SHAPECREATER_H
#define GEOPAD_SHAPECREATER_H

#include "Shape.h"
#include "Point.h"
#include "Circle.h"
#include "Line.h"
#include "Rect.h"
#include "Polygon.h"
#include <unordered_set>

class ShapeCreater {
private:
    std::unordered_set<Shape *> shapes;
public:
    ShapeCreater() = default;

    ~ShapeCreater();

    Point *CreatePoint(double, double, bool= false);

    Point *CreatePoint(Point *);

    Circle *CreateCircle(Point *, Point *);

    Rect *CreateRect(Point *, Point *);

    Line *CreateLine(Point *, Point *);

    Polygon *CreatePolygon(vector<Point *> &);

    void DestroyShape(Shape *);
};

#endif //GEOPAD_SHAPECREATER_H
