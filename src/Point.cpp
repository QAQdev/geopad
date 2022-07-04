
#include "include/Point.h"
#include <GLFW/glfw3.h>

Point::Point()
    :x(0.0f),
    y(0.0f),
    Shape(0.063,0.3,0.54) {
    is_negative = false;
    id = HashId::UnivId();
}

Point::Point(double _x, double _y,bool _is_negative)
    :x(_x),
    y(_y),
    Shape(0.063,0.3,0.54) {
    is_negative = _is_negative;
    if (is_negative) {
        r = g = b = 0.41;
    }
    id = HashId::UnivId();
}

Point::~Point() {

}

void Point::Draw() const {
    glColor4f(r, g, b, 0);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

double Point::GetXCoordinate() const {
    return x;
}

double Point::GetYCoordinate() const {
    return y;
}

void Point::SetXCoordinate(const double _x) {
    x = _x;
}

void Point::SetYCoordinate(const double _y) {
    y = _y;
}

unsigned int Point::GetId() const {
    return id;
}

void Point::SetRGB(double _r, double _g, double _b) {
    r = _r;
    g = _g;
    b = _b;
}

bool Point::IfSelect(GLFWwindow* window, double mx, double my) {
    int threshold = 10; //20 pixel
    int win_w, win_h;
    glfwGetWindowSize(window, &win_w, &win_h);
    return fabs(Frame_2_Pixel_X(x, win_w) - mx) <= threshold && fabs(Frame_2_Pixel_Y(y, win_h) - my) <= threshold;
}

ShapeType Point::GetShapeType() {
    return ShapeType::POINT;
}

void Point::SetName(const string &name) {
    text = name;
}

double Point::DistSquare(const Point *p1, const Point *p2) {
    return (p1->x - p2->x) * (p1->x - p2->x) + (p1->y - p2->y) * (p1->y - p2->y);
}

void Point::SetNegative() {
    is_negative = true;
}
