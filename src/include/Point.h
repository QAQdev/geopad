#ifndef GEOGEBRA_POINT_H
#define GEOGEBRA_POINT_H

#include <GLFW/glfw3.h>

#include <cmath>
#include <map>

#include "Shape.h"
#include "Config.h"

using namespace std;

/**
 * @param x x_coordinate
 * @param y y_coordinate
 */

class Point: virtual public Shape {
private:
    double x, y;
    bool is_negative = false;

public:
    Point();

    Point(double, double, bool= false);

    ~Point() override;

    void Draw() const override;

    double GetXCoordinate() const;

    double GetYCoordinate() const;

    void SetXCoordinate(double);

    void SetYCoordinate(double);

    unsigned int GetId() const override;

    void SetRGB(double, double, double) override;

    void SetName(const string &) override;

    ShapeType GetShapeType() override;

    bool IfSelect(GLFWwindow *, double, double) override;

    void SetNegative();

    bool IfNegative() const { return is_negative; }

    static double DistSquare(const Point *, const Point *);
};

#endif //GEOGEBRA_POINT_H
