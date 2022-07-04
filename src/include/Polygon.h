#ifndef GEOPAD_POLYGON_H
#define GEOPAD_POLYGON_H

#include "Shape.h"
#include "Point.h"
#include "Line.h"
#include <vector>

class Polygon: public Shape {
private:
    std::vector<Point *> points_on_edge;

    Polygon() = default;

public:

    Polygon(std::vector<Point *>&);

    ~Polygon();

    void Draw() const override;

    std::vector<Point *> &GetPointsOnEdge();

    unsigned int GetId() const override;

    void SetType(Type) override;

    void SetRGB(double, double, double) override;

    void SetFill() override;

    void SetName(const string &) override;

    ShapeType GetShapeType() override;

    bool IfSelect(GLFWwindow *, double, double) override;
};


#endif //GEOPAD_POLYGON_H
