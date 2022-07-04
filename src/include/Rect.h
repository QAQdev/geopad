#ifndef GEOGEBRA_RECT_H
#define GEOGEBRA_RECT_H

#include "Shape.h"
#include "Point.h"

class Rect: public Shape {
private:
    /**
     * two points on one edge
     */
    Point *top_left;
    Point *lower_right;

    Rect() = default;

public:
    Rect(Point *, Point *);

    ~Rect() override;

    void Draw() const override;

    Point *GetTopLeft() const;

    Point *GetLowerRight() const;

    double GetWidth() const;

    double GetHeight() const;

    unsigned int GetId() const override;

    void SetType(Type) override;

    void SetRGB(double, double, double) override;

    void SetFill() override;

    ShapeType GetShapeType() override;

    bool IfSelect(GLFWwindow *, double, double) override;
};


#endif //GEOGEBRA_RECT_H
