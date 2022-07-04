//
// Created by kenshin on 5/18/22.
//

#include "include/Polygon.h"


Polygon::Polygon(std::vector<Point *>&list)
    :points_on_edge(list),
    Shape(0.094,0.45,0.80) {
    id = HashId::UnivId();
    set_fill = true;
}

Polygon::~Polygon() {

}

void Polygon::Draw() const {

    if (type == Type::DOTTED) {
        glLineStipple(10, 0x5555);
        glEnable(GL_LINE_STIPPLE);
    }
    glColor4f(r, g, b, 0.2);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT, GL_FILL);
    glEnable(GL_POLYGON_SMOOTH);
    glBegin(GL_POLYGON);
    for (auto &it: points_on_edge) {
        glVertex2f(it->GetXCoordinate(), it->GetYCoordinate());
    }
    glEnd();

    glColor4f(r, g, b, 0.7);
    glBegin(GL_LINE_LOOP);
    for (auto &it: points_on_edge) {
        glVertex2f(it->GetXCoordinate(), it->GetYCoordinate());
    }
    glEnd();
    glDisable(GL_BLEND);
}

std::vector<Point *> &Polygon::GetPointsOnEdge() {
    return points_on_edge;
}

unsigned int Polygon::GetId() const {
    return id;
}

void Polygon::SetType(Type _type) {
    type = _type;
}

void Polygon::SetRGB(double _r, double _g, double _b) {
    r = _r;
    g = _g;
    b = _b;
}

void Polygon::SetFill() {
    set_fill = !set_fill;
}

void Polygon::SetName(const string &_name) {
    text = _name;
}

ShapeType Polygon::GetShapeType() {
    return ShapeType::POLYGON;
}

bool Polygon::IfSelect(GLFWwindow *canvas, double mx, double my) {
    auto cur = *points_on_edge.begin(), next = cur;
    for (auto it = points_on_edge.begin(); it != points_on_edge.end(); it++) {
        cur = *it, next = *(it + 1);
        if (it + 1 == points_on_edge.end()) {
            auto line = Line(cur, points_on_edge[0]);
            if (line.IfSelect(canvas, mx, my)) return true;
            else return false;
        }
        auto line = Line(cur, next);
        if (line.IfSelect(canvas, mx, my)) return true;
    }
    return false;
}