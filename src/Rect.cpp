//
// Created by kenshin on 5/1/22.
//

#include <cmath>
#include <GLFW/glfw3.h>
#include "include/Rect.h"

Rect::Rect(Point * _left, Point *_right)
    :top_left(_left),
    lower_right(_right),
     Shape(0.41,0.41,0.41) {
    id = HashId::UnivId();
}

Rect::~Rect() {

}

void Rect::Draw()const {
    glColor4f(r, g, b, 0.7);
    if (type == Type::DOTTED) {
        glLineStipple(10, 0x5555);
        glEnable(GL_LINE_STIPPLE);
    }

    glBegin(GL_LINE_LOOP);
    glVertex2f(top_left->GetXCoordinate(), top_left->GetYCoordinate());
    glVertex2f(lower_right->GetXCoordinate(), top_left->GetYCoordinate());

    glVertex2f(lower_right->GetXCoordinate(), top_left->GetYCoordinate());
    glVertex2f(lower_right->GetXCoordinate(), lower_right->GetYCoordinate());

    glVertex2f(lower_right->GetXCoordinate(), lower_right->GetYCoordinate());
    glVertex2f(top_left->GetXCoordinate(), lower_right->GetYCoordinate());

    glEnd();

    glDisable(GL_LINE_STIPPLE);
}

double Rect::GetWidth() const {
    return fabs(top_left->GetXCoordinate() - lower_right->GetXCoordinate());
}

double Rect::GetHeight() const {
    return fabs(top_left->GetYCoordinate() - lower_right->GetYCoordinate());
}

unsigned int Rect::GetId() const {
    return id;
}

void Rect::SetType(Type _type) {
    type = _type;
}

void Rect::SetRGB(double _r, double _g, double _b) {
    r = _r;
    g = _g;
    b = _b;
}

void Rect::SetFill() {
    set_fill = !set_fill;
}

Point *Rect::GetTopLeft() const {
    return top_left;
}

Point *Rect::GetLowerRight() const {
    return lower_right;
}

bool Rect::IfSelect(GLFWwindow *window, double mx, double my) {
    int win_w, win_h;
    glfwGetWindowSize(window, &win_w, &win_h);
    double px = Pixel_X_2_Frame_X(mx, win_w),
            py = Pixel_Y_2_Frame_Y(my, win_h);

    Point tmp(top_left->GetXCoordinate(), lower_right->GetYCoordinate());

    double dist1 = fabs((px - top_left->GetXCoordinate())),
            dist2 = fabs(py - lower_right->GetYCoordinate()),
            dist3 = fabs(px - lower_right->GetXCoordinate()),
            dist4 = fabs(py - top_left->GetYCoordinate());

    return dist1 <= THRESHOLD || dist2 <= THRESHOLD || dist3 <= THRESHOLD || dist4 <= THRESHOLD;
}

ShapeType Rect::GetShapeType() {
    return ShapeType::RECT;
}
