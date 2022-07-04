#ifndef GEOGEBRA_LINE_H
#define GEOGEBRA_LINE_H

#include "Shape.h"
#include "Point.h"

class Line: public Shape {
private:
    Point *src, *dst;
    bool src_msk, dst_msk;

    Line() = default;

public:
    Line(Point *, Point *, bool = false, bool = false);

    ~Line() override;

    void Draw() const override;

    double GetX1Coordinate() const;

    double GetX2Coordinate() const;

    double GetY1Coordinate() const;

    double GetY2Coordinate() const;

    Point *GetSrc() const;

    Point *GetDst() const;

    unsigned int GetId() const override;

    ShapeType GetShapeType() override;

    bool IfSelect(GLFWwindow *, double, double) override;

    void SetRGB(double, double, double) override;

    void SetType(Type _type) override;
};


#endif //GEOGEBRA_LINE_H
