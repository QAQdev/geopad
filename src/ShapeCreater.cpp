//
// Created by kenshin on 5/3/22.
//

#include "include/ShapeCreater.h"

ShapeCreater::~ShapeCreater() {
    for (auto it: shapes) {
        delete it;
    }
}

Point *ShapeCreater::CreatePoint(double x, double y,bool is_negative) {
    Point *p = new Point(x, y, is_negative);
    shapes.insert(p);
    return p;
}

Point* ShapeCreater::CreatePoint(Point*p) {
    Point *q = p;
    return q;
}

Circle *ShapeCreater::CreateCircle(Point *p1, Point *p2) {
    Circle *p = new Circle(p1, p2);
    shapes.insert(p);
    return p;
}

Rect *ShapeCreater::CreateRect(Point *p1, Point *p2) {
    Rect *p = new Rect(p1, p2);
    shapes.insert(p);
    return p;
}

Line *ShapeCreater::CreateLine(Point *p1, Point *p2) {
    Line *p = new Line(p1, p2);
    shapes.insert(p);
    return p;
}

Polygon *ShapeCreater::CreatePolygon(vector<Point *> &list) {
    Polygon *p = new Polygon(list);
    shapes.insert(p);
    return p;
}

void ShapeCreater::DestroyShape(Shape*p) {
    if (p == nullptr) return;

    auto iter = shapes.find(p);
    if (iter != shapes.end()) {
        shapes.erase(p);
    }
}