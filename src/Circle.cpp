//
// Created by kenshin on 5/1/22.
//

#include <cmath>
#include "include/Circle.h"

#define N 7200

Circle::Circle(Point *p1, Point *p2)
    :center(p1),
    Shape(0.41,0.41,0.41) {
    on_circle.push_back(p2);
    id = HashId::UnivId();
}

Circle::~Circle() {

}

void Circle::Draw() const {
    glColor4f(r, g, b, 0.7);
    if (type == Type::DOTTED) {
        glLineStipple(10, 0x5555);
        glEnable(GL_LINE_STIPPLE);
    }
    glEnable(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (!set_fill) glBegin(GL_LINE_LOOP);
    else glBegin(GL_POLYGON);
    double r = GetRadius();
    double cx = GetCenterXCoordinate(),
            cy = GetCenterYCoordinate();
    for (int j = 0; j < N; j++) {
        glVertex2f(cx + r * cos(2 * M_PI * j / N),
                   cy + r * sin(2 * M_PI * j / N));   //定义顶点
    }
    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_LINE_STIPPLE);
}

double Circle::GetCenterXCoordinate() const {
    return center->GetXCoordinate();
}

double Circle::GetCenterYCoordinate() const {
    return center->GetYCoordinate();
}

double Circle::GetRadius() const {
    Point *p = on_circle[0];
    return sqrt(pow(center->GetXCoordinate() - p->GetXCoordinate(), 2) +
                pow(center->GetYCoordinate() - p->GetYCoordinate(), 2));
}

unsigned int Circle::GetId() const {
    return id;
}

void Circle::SetType(Type _type) {
    type = _type;
}

void Circle::SetRGB(double _r, double _g, double _b) {
    r = _r;
    g = _g;
    b = _b;
}

void Circle::SetFill() {
    set_fill = !set_fill;
}

void Circle::SetName(const string &_name) {
    text = _name;
}

Point *Circle::GetCenter() const {
    return center;
}

bool Circle::IfSelect(GLFWwindow *window, double mx, double my) {
    int win_w, win_h;
    glfwGetWindowSize(window, &win_w, &win_h);
    double px = Pixel_X_2_Frame_X(mx, win_w),
            py = Pixel_Y_2_Frame_Y(my, win_h);
    double dist_square = fabs((px - center->GetXCoordinate()) * (px - center->GetXCoordinate())
                              + (py - center->GetYCoordinate()) * (py - center->GetYCoordinate()) -
                              GetRadius() * GetRadius());
    return dist_square <= THRESHOLD;
}

ShapeType Circle::GetShapeType() {
    return ShapeType::CIRCLE;
}

Point *Circle::GetPointOnArc() const {
    return on_circle[0];
}
