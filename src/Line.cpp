//
// Created by kenshin on 5/1/22.
//

#include <GLFW/glfw3.h>
#include "include/Line.h"

Line::Line(Point *p1, Point *p2, bool _src_msk, bool _dst_msk)
    :src(p1),
    dst(p2),
    src_msk(_src_msk),
    dst_msk(_dst_msk),
    Shape(0.41,0.41,0.41) {
    id = HashId::UnivId();
}

Line::~Line() {

}

void Line::Draw() const {
    glColor4f(r, g, b, 0.7);
    glEnable(GL_LINE_SMOOTH);
    if (type == Type::DOTTED) {
        glLineStipple(10, 0x5555);
        glEnable(GL_LINE_STIPPLE);
    }
    double A, B, C;
    A = src->GetYCoordinate() - dst->GetYCoordinate();
    B = -(src->GetXCoordinate() - dst->GetXCoordinate());
    C = src->GetXCoordinate() * dst->GetYCoordinate() - dst->GetXCoordinate() * src->GetYCoordinate();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_LINES);

    if (src_msk && dst_msk) {
        glVertex2f(src->GetXCoordinate(), src->GetYCoordinate());
        glVertex2f(dst->GetXCoordinate(), dst->GetYCoordinate());
    } else if (!src_msk && !dst_msk) {
        glVertex2f((B - C) / A, -1);
        glVertex2f(-(B + C) / A, 1);
    }

    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_LINE_STIPPLE);
}

double Line::GetX1Coordinate() const {
    return src->GetXCoordinate();
}

double Line::GetX2Coordinate() const {
    return dst->GetXCoordinate();
}

double Line::GetY1Coordinate() const {
    return src->GetYCoordinate();
}

double Line::GetY2Coordinate() const {
    return dst->GetYCoordinate();
}

unsigned int Line::GetId() const {
    return id;
}

Point *Line::GetSrc() const {
    return src;
}

Point *Line::GetDst() const {
    return dst;
}

bool Line::IfSelect(GLFWwindow *window, double mx, double my) {
    int win_w, win_h;
    glfwGetWindowSize(window, &win_w, &win_h);
    double A, B, C;
    A = src->GetYCoordinate() - dst->GetYCoordinate();
    B = -(src->GetXCoordinate() - dst->GetXCoordinate());
    C = src->GetXCoordinate() * dst->GetYCoordinate() - dst->GetXCoordinate() * src->GetYCoordinate();
    double px = Pixel_X_2_Frame_X(mx, win_w),
            py = Pixel_Y_2_Frame_Y(my, win_h);
    double dist_square = (A * px + B * py + C) * (A * px + B * py + C) / (A * A + B * B);
    return dist_square <= THRESHOLD;
}

ShapeType Line::GetShapeType() {
    return ShapeType::LINE;
}

void Line::SetRGB(double _r, double _g, double _b) {
    r = _r;
    g = _g;
    b = _b;
}

void Line::SetType(Type _type) {
    type = _type;
}
