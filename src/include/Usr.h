#ifndef GEOPAD_USR_H
#define GEOPAD_USR_H

#include <iostream>
#include <queue>
#include "ShapeCreater.h"

/**
 * User mode
 */

class UsrMode {
public:
    static bool move;
    static bool circle;
    static bool rect;
    static bool point;
    static bool line;
    static bool polygon;
    static bool delete_;
    static bool midpoint;
    static bool vertical;
    static bool tangent;
    static bool reflect_about_point;
public:
    static void SetMode(bool &mode) {
        move = delete_
                = circle = rect = point = line = polygon
                = midpoint = vertical = tangent
                = reflect_about_point = false;
        mode = true;
    }
};

/**
 * Status when do adding shapes
 */

class Status {
public:
    static bool is_dragged;
    static bool is_selected;
    static int add_circle_step;
    static int add_line_step;
    static int add_rect_step;
    static int add_polygon_step;
public:
    static void InitAllStatus() {
        is_dragged = false;
        is_selected = false;
        add_polygon_step = NOT_ADDING;
        add_circle_step = NOT_ADDING;
        add_line_step = NOT_ADDING;
        add_rect_step = NOT_ADDING;
    }
};

class Canvas;
class IO;

/**
 * Do adding, deleting, constructing or transforming
 */

class Usr {
private:
    friend class Canvas;

    friend class IO;

    ShapeCreater shape_creater;
    Shape *to_be_deleted{nullptr};
    Shape *to_be_edited{nullptr};
public:
    void DragPoint(Canvas *, double, double);

public:
    void TryCreateCircle(Canvas *, double, double);

    void TryCreateLine(Canvas *, double, double);

    void TryCreateRect(Canvas *, double, double);

    void TryCreatePoint(Canvas *, double, double);

    void TryCreatePolygon(Canvas *, double, double);

public:
    /**
    * cascade delete
    */
    void TryDelete(Canvas *);

    bool SelectShape(Canvas *, double, double);

private:
    void TryDeletePointRelatedShape(Canvas *, const Point *);

public:
    /**
     * construct: midpoint
     */
    void TryConstructCenter(Canvas *, double, double);

    /**
     * construct: vertical
     */
    void TryConstructVertical(Canvas *, double, double);

    /**
     * construct: tangent
     */
    void TryConstructTangent(Canvas *, double, double);

    /**
     * transform: reflect about point
     */
    void TryTransformReflectAboutPoint(Canvas *, double, double);

private:
    /**
     * add a single point, the first step of adding a shape
     */
    void TryCreateStep1(Canvas *, double, double, Point *&);

    /**
     * add a specific graphic, the second step of adding a shape
     */
    void TryCreateStep2(Canvas *, double, double, Point *&, Point *&);
};

#endif //GEOPAD_USR_H
