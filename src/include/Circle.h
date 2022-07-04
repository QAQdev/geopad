#ifndef GEOGEBRA_CIRCLE_H
#define GEOGEBRA_CIRCLE_H

#include "Shape.h"
#include "Point.h"

using namespace std;

class Circle: public Shape {
private:
    Point *center;
    vector<Point *> on_circle;

    Circle() = default;

public:

    Circle(Point *, Point *);

    ~Circle() override;

    void Draw() const override;

    Point *GetCenter() const;

    Point *GetPointOnArc() const;

    double GetCenterXCoordinate() const;

    double GetCenterYCoordinate() const;

    double GetRadius() const;

    unsigned int GetId() const override;

    void SetType(Type) override;

    void SetRGB(double, double, double) override;

    void SetFill() override;

    void SetName(const string &) override;

    ShapeType GetShapeType() override;

    bool IfSelect(GLFWwindow *, double, double) override;
};

#endif //GEOGEBRA_CIRCLE_H
