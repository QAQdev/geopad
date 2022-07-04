//
// Created by kenshin on 5/16/22.
//
#include "include/Canvas.h"
#include "include/Usr.h"

bool UsrMode::move = false;
bool UsrMode::delete_ = false;
bool UsrMode::circle = false;
bool UsrMode::rect = false;
bool UsrMode::point = false;
bool UsrMode::line = false;
bool UsrMode::polygon = false;
bool UsrMode::midpoint = false;
bool UsrMode::vertical = false;
bool UsrMode::tangent = false;
bool UsrMode::reflect_about_point = false;

bool Status::is_dragged = false;
bool Status::is_selected = false;
int Status::add_circle_step = NOT_ADDING;
int Status::add_line_step = NOT_ADDING;
int Status::add_rect_step = NOT_ADDING;
int Status::add_polygon_step = NOT_ADDING;

/** Creation */

void Usr::TryCreateCircle(Canvas*canvas,double x_pos,double y_pos) {

    static Point *p1, *p2;

    if (Status::add_circle_step == NOT_ADDING) {
        Status::add_circle_step = ADD_CENTER;
        TryCreateStep1(canvas, x_pos, y_pos, p1);
    } else if (Status::add_circle_step == ADD_CENTER) { // the second point on arc
        Status::add_circle_step = NOT_ADDING;
        TryCreateStep2(canvas, x_pos, y_pos, p1, p2);
    }
}

void Usr::TryCreateLine(Canvas *canvas, double x_pos, double y_pos) {
    static Point *p1, *p2;

    if (Status::add_line_step == NOT_ADDING) {
        Status::add_line_step = ADD_FIRST_POINT;
        TryCreateStep1(canvas, x_pos, y_pos, p1);
    } else if (Status::add_line_step == ADD_FIRST_POINT) {
        Status::add_line_step = NOT_ADDING;
        TryCreateStep2(canvas, x_pos, y_pos, p1, p2);
    }
}

void Usr::TryCreateRect(Canvas *canvas, double x_pos, double y_pos) {
    static Point *p1, *p2;

    if (Status::add_rect_step == NOT_ADDING) {
        Status::add_rect_step = ADD_FIRST_POINT;
        TryCreateStep1(canvas, x_pos, y_pos, p1);
    } else if (Status::add_rect_step == ADD_FIRST_POINT) {
        Status::add_rect_step = NOT_ADDING;
        TryCreateStep2(canvas, x_pos, y_pos, p1, p2);
    }
}

void Usr::TryCreatePolygon(Canvas *canvas, double x_pos, double y_pos) {
    static Point *p1, *p2;
    static vector<Point *> list;

    if (Status::add_polygon_step == NOT_ADDING) {
        Status::add_polygon_step = POLYGON_ADDING;
        TryCreateStep1(canvas, x_pos, y_pos, p1);
        list.push_back(p1);
    } else if (Status::add_polygon_step == POLYGON_ADDING) {
        TryCreateStep1(canvas, x_pos, y_pos, p2);
        auto point = list.begin();
        for (; point != list.end(); point++) {
            if (*point == p2) break;
        }

        if (point == list.begin()) {
            if (list.size() + 1 >= 3) {
                auto new_polygon = shape_creater.CreatePolygon(list);
                canvas->polygons[new_polygon->GetId()] = new_polygon;
                list.clear();
                Status::add_polygon_step = NOT_ADDING;
            } else {
                for (auto &it: list) {
                    shape_creater.DestroyShape(it);
                }
                shape_creater.DestroyShape(p2);
                list.clear();
                Status::add_polygon_step = NOT_ADDING;
            }
        } else if (point == list.end()) {
            list.push_back(p2);
        } else {
            for (auto &it: list) {
                shape_creater.DestroyShape(it);
            }
            shape_creater.DestroyShape(p2);
            list.clear();
            Status::add_polygon_step = NOT_ADDING;
        }
    }
}

void Usr::TryCreatePoint(Canvas *canvas, double x_pos, double y_pos) {
    static Point *p1;
    TryCreateStep1(canvas, x_pos, y_pos, p1);
}

void Usr::TryCreateStep1(Canvas *canvas, double x_pos, double y_pos, Point*&p1) {
    int win_w = canvas->width, win_h = canvas->height;

    p1 = shape_creater.CreatePoint(Pixel_X_2_Frame_X(x_pos, win_w),
                                   Pixel_Y_2_Frame_Y(y_pos, win_h));

    auto point = canvas->points.begin();
    for (; point != canvas->points.end(); point++) {
        if (point->second->IfSelect(canvas->window,
                                    Frame_2_Pixel_X(p1->GetXCoordinate(), win_w),
                                    Frame_2_Pixel_Y(p1->GetYCoordinate(), win_h))) {
            break;
        }
    }
    if (point != canvas->points.end()) {
        shape_creater.DestroyShape(p1);
        p1 = point->second;
    } else {
        canvas->points[p1->GetId()] = p1; // insert to point list
    }
}

void Usr::TryCreateStep2(Canvas *canvas, double x_pos, double y_pos, Point *&p1,Point*&p2) {
    int win_w = canvas->width, win_h = canvas->height;

    p2 = shape_creater.CreatePoint(Pixel_X_2_Frame_X(x_pos, win_w),
                                   Pixel_Y_2_Frame_Y(y_pos, win_h));

    auto point = canvas->points.begin();
    for (; point != canvas->points.end(); point++) {
        if (point->second->IfSelect(canvas->window,
                                    Frame_2_Pixel_X(p2->GetXCoordinate(), win_w),
                                    Frame_2_Pixel_Y(p2->GetYCoordinate(), win_h))) {
            // reach the last, same as p1, do not create
            if (point->second == p1) {
                // p1 is not in the point list, do deletion
                if (canvas->points.find(p1->GetId()) == canvas->points.end()) {
                    Canvas::points.erase(p1->GetId());
                }
                shape_creater.DestroyShape(p1);
            } else {
                if (UsrMode::circle) {
                    auto new_circle = shape_creater.CreateCircle(p1, point->second);
                    Canvas::circles[new_circle->GetId()] = new_circle;
                } else if (UsrMode::rect) {
                    auto new_rect = shape_creater.CreateRect(p1, point->second);
                    Canvas::rects[new_rect->GetId()] = new_rect;
                } else if (UsrMode::line) {
                    auto new_line = shape_creater.CreateLine(p1, point->second);
                    Canvas::lines[new_line->GetId()] = new_line;
                }
            }
            shape_creater.DestroyShape(p2);
            break;
        }
    }

    if (point == canvas->points.end()) {
        Canvas::points[p2->GetId()] = p2;
        if (UsrMode::circle) {
            auto new_circle = shape_creater.CreateCircle(p1, p2);
            Canvas::circles[new_circle->GetId()] = new_circle;
        } else if (UsrMode::rect) {
            auto new_rect = shape_creater.CreateRect(p1, p2);
            Canvas::rects[new_rect->GetId()] = new_rect;
            cout << "hello";
        } else if (UsrMode::line) {
            auto new_line = shape_creater.CreateLine(p1, p2);
            Canvas::lines[new_line->GetId()] = new_line;
        }
    }
}

/** Deletion */

void Usr::TryDeletePointRelatedShape(Canvas*canvas,const Point*p) {
    vector<Shape *> related;

    for (auto &circle: canvas->circles) {
        if (circle.second->GetCenter() == p
            || circle.second->GetPointOnArc() == p) {
            related.push_back(circle.second);
            shape_creater.DestroyShape(circle.second);
        }
    }

    for (auto &it: related) {
        canvas->circles.erase(it->GetId());
    }
    related.clear();

    for (auto &rect: canvas->rects) {
        if (rect.second->GetTopLeft() == p
            || rect.second->GetLowerRight() == p) {
            related.push_back(rect.second);
            shape_creater.DestroyShape(rect.second);
        }
    }

    for (auto &it: related) {
        canvas->rects.erase(it->GetId());
    }
    related.clear();

    for (auto &line: canvas->lines) {
        if (line.second->GetSrc() == p
            || line.second->GetDst() == p) {
            related.push_back(line.second);
            shape_creater.DestroyShape(line.second);
        }
    }

    for (auto &it: related) {
        canvas->lines.erase(it->GetId());
    }
    related.clear();

    for (auto &polygon: canvas->polygons) {
        auto point_list = polygon.second->GetPointsOnEdge();
        for (auto &point: point_list) {
            if (point == p) {
                related.push_back(polygon.second);
                shape_creater.DestroyShape(polygon.second);
                break;
            }
        }
    }

    for (auto &it: related) {
        canvas->polygons.erase(it->GetId());
    }
    related.clear();
}

void Usr::TryDelete(Canvas *canvas) {
    if (to_be_deleted == nullptr) return;

    if (to_be_deleted->GetShapeType() == ShapeType::POINT) {

        Point *midpoint, *pedal, *p_tangent1, *p_tangent2;
        queue<Point *> deleted_queue;

        /**
         * BFS, cascade delete
         */
        deleted_queue.push(dynamic_cast<Point *>(to_be_deleted));
        while (!deleted_queue.empty()) {

            auto p = deleted_queue.front();
            deleted_queue.pop();

            for (auto it = canvas->midpoint.begin(); it != canvas->midpoint.end();) {
                if (p == it->second.first || p == it->second.second) {
                    midpoint = canvas->points[it->first];
                    canvas->points.erase(it->first);
                    deleted_queue.push(midpoint);
                    shape_creater.DestroyShape(midpoint);
                    canvas->midpoint.erase(it++);
                } else it++;
            }

            for (auto it = canvas->vertical.begin(); it != canvas->vertical.end();) {
                if (p == it->second.first ||
                    p == it->second.second->GetSrc() || p == it->second.second->GetDst()) {
                    pedal = canvas->points[it->first];
                    canvas->points.erase(it->first);
                    deleted_queue.push(pedal);
                    shape_creater.DestroyShape(pedal);
                    canvas->vertical.erase(it++);
                } else it++;
            }

            for (auto it = canvas->tangent.begin(); it != canvas->tangent.end();) {
                if (p == canvas->points[it->first.first] || p == canvas->points[it->first.second] ||
                    p == it->second.first || p == it->second.second->GetCenter() ||
                    p == it->second.second->GetPointOnArc()) {

                    p_tangent1 = canvas->points[it->first.first];

                    canvas->points.erase(p_tangent1->GetId());
                    deleted_queue.push(p_tangent1);
                    shape_creater.DestroyShape(p_tangent1);

                    if (it->first.second != 0) {
                        p_tangent2 = canvas->points[it->first.second];
                        canvas->points.erase(p_tangent2->GetId());
                        deleted_queue.push(p_tangent2);
                        shape_creater.DestroyShape(p_tangent2);
                    }

                    canvas->tangent.erase(it++);
                } else it++;
            }

            TryDeletePointRelatedShape(canvas, p);
        }

        canvas->points.erase(to_be_deleted->GetId());
        shape_creater.DestroyShape(to_be_deleted);

    } else if (to_be_deleted->GetShapeType() == ShapeType::CIRCLE) {
        canvas->circles.erase(to_be_deleted->GetId());
        for (auto it = canvas->reflect_about_point.begin(); it != canvas->reflect_about_point.end();) {
            if (to_be_deleted == it->second.second) {
                canvas->circles.erase(dynamic_cast<Circle *>(it->first)->GetId());
                canvas->reflect_about_point.erase(it++);
            } else it++;
        }
    } else if (to_be_deleted->GetShapeType() == ShapeType::LINE) {
        canvas->lines.erase(to_be_deleted->GetId());
    } else if (to_be_deleted->GetShapeType() == ShapeType::RECT) {
        canvas->rects.erase(to_be_deleted->GetId());
    } else if (to_be_deleted->GetShapeType() == ShapeType::POLYGON) {
        canvas->polygons.erase(to_be_deleted->GetId());
    }
}

bool Usr::SelectShape(Canvas *canvas, double x_pos, double y_pos) {

    for (auto &point: canvas->points) {
        if (point.second->IfSelect(canvas->window, x_pos, y_pos)) {
            this->to_be_edited = this->to_be_deleted = point.second;
            return true;
        }
    }

    for (auto &circle: canvas->circles) {
        if (circle.second->IfSelect(canvas->window, x_pos, y_pos)) {
            this->to_be_edited = this->to_be_deleted = circle.second;
            return true;
        }
    }

    for (auto &rect: canvas->rects) {
        if (rect.second->IfSelect(canvas->window, x_pos, y_pos)) {
            this->to_be_edited = this->to_be_deleted = rect.second;
            return true;
        }
    }

    for (auto &line: canvas->lines) {
        if (line.second->IfSelect(canvas->window, x_pos, y_pos)) {
            this->to_be_edited = this->to_be_deleted = line.second;
            return true;
        }
    }

    for (auto &polygon: canvas->polygons) {
        if (polygon.second->IfSelect(canvas->window, x_pos, y_pos)) {
            this->to_be_edited = this->to_be_deleted = polygon.second;
            return true;
        }
    }
    return false;
}

/**
* Move
*/

void Usr::DragPoint(Canvas *canvas, double mx, double my) {
    for (auto &point: canvas->points) {
        if (point.second->IfSelect(canvas->window, mx, my) && !point.second->IfNegative()) {
            point.second->SetXCoordinate(Pixel_X_2_Frame_X(mx, canvas->width));
            point.second->SetYCoordinate(Pixel_Y_2_Frame_Y(my, canvas->height));
            break;
        }
    }

    /**
     * update geometry relation
     */
    // midpoint
    for (auto it = canvas->midpoint.begin(); it != canvas->midpoint.end(); it++) {
        auto point_pair = it->second;
        canvas->points[it->first]->SetXCoordinate(
                point_pair.first->GetXCoordinate() / 2 + point_pair.second->GetXCoordinate() / 2);
        canvas->points[it->first]->SetYCoordinate(
                point_pair.first->GetYCoordinate() / 2 + point_pair.second->GetYCoordinate() / 2);
    }

    // vertical
    double A, B, C;
    for (auto it = canvas->vertical.begin(); it != canvas->vertical.end(); it++) {
        auto negative_point = canvas->points[it->first];
        auto point = canvas->points[it->second.first->GetId()];
        auto line = it->second.second;
        A = line->GetY1Coordinate() - line->GetY2Coordinate();
        B = -(line->GetX1Coordinate() - line->GetX2Coordinate());
        C = line->GetX1Coordinate() * line->GetY2Coordinate() - line->GetX2Coordinate() * line->GetY1Coordinate();

        if (fabs(A * point->GetXCoordinate() + B * point->GetYCoordinate() + C) <= PRECISION) {
            negative_point->SetXCoordinate((B * point->GetXCoordinate() - A * point->GetYCoordinate() - A) / B);
            negative_point->SetYCoordinate(-1);
        } else {
            negative_point->SetXCoordinate(
                    (B * B * point->GetXCoordinate() - A * B * point->GetYCoordinate() - A * C) / (A * A + B * B));
            negative_point->SetYCoordinate(
                    (A * A * point->GetYCoordinate() - A * B * point->GetXCoordinate() - B * C) / (A * A + B * B));
        }
    }

    //tangent
    for (auto it = canvas->tangent.begin(); it != canvas->tangent.end(); it++) {
        auto p_tangent1 = canvas->points[it->first.first];
        auto circle = it->second.second;
        auto outside = it->second.first;
        // only one tangent point
        if (it->first.second == 0) {
            Line l(circle->GetCenter(), outside);
            A = l.GetY1Coordinate() - l.GetY2Coordinate();
            B = -(l.GetX1Coordinate() - l.GetX2Coordinate());

            p_tangent1->SetXCoordinate((B * outside->GetXCoordinate() - A * outside->GetYCoordinate() - A) / B);
        } else {
            auto p_tangent2 = canvas->points[it->first.second];
            auto dist = sqrt(Point::DistSquare(circle->GetCenter(), outside));
            // length from outside to p_tangent
            double length = sqrt(dist * dist - circle->GetRadius() * circle->GetRadius());
            // vector: outside to c
            ImVec2 v((circle->GetCenterXCoordinate() - outside->GetXCoordinate()) / dist,
                     (circle->GetCenterYCoordinate() - outside->GetYCoordinate()) / dist);
            // angle between tangent and c
            auto angle = asin(circle->GetRadius() / dist);

            double x1, y1, x2, y2;
            x1 = v.x * cos(angle) - v.y * sin(angle), y1 = v.x * sin(angle) + v.y * cos(angle);
            x2 = v.x * cos(-angle) - v.y * sin(-angle), y2 = v.x * sin(-angle) + v.y * cos(-angle);

            p_tangent1->SetXCoordinate(x1 * length + outside->GetXCoordinate());
            p_tangent1->SetYCoordinate(y1 * length + outside->GetYCoordinate());
            p_tangent2->SetXCoordinate(x2 * length + outside->GetXCoordinate());
            p_tangent2->SetYCoordinate(y2 * length + outside->GetYCoordinate());
        }
    }

    // reflect about point
    for (auto it = canvas->reflect_about_point.begin(); it != canvas->reflect_about_point.end(); it++) {

        auto pivot = it->second.first;
        auto circle = it->second.second;

        if (it->first->GetShapeType() == ShapeType::CIRCLE) {
            Circle *reflected;
            reflected = canvas->circles[it->first->GetId()];

            reflected->GetCenter()->SetXCoordinate(2 * pivot->GetXCoordinate() - circle->GetCenterXCoordinate());
            reflected->GetCenter()->SetYCoordinate(2 * pivot->GetYCoordinate() - circle->GetCenterYCoordinate());

            reflected->GetPointOnArc()->SetXCoordinate(
                    2 * pivot->GetXCoordinate() - circle->GetPointOnArc()->GetXCoordinate());
            reflected->GetPointOnArc()->SetYCoordinate(
                    2 * pivot->GetYCoordinate() - circle->GetPointOnArc()->GetYCoordinate());
        }
    }
}

/**
* Construction
*/

void Usr::TryConstructCenter(Canvas*canvas,double x_pos,double y_pos) {
    static Point *p1{nullptr}, *p2{nullptr};
    Point *center;

    if (p1 == nullptr && p2 == nullptr) {
        for (auto &point: canvas->points) {
            if (point.second->IfSelect(canvas->window, x_pos, y_pos)) {
                p1 = point.second;
                return;
            }
        }
    } else if (p1 != nullptr && p2 == nullptr) {
        for (auto &point: canvas->points) {
            if (point.second->IfSelect(canvas->window, x_pos, y_pos)) {
                p2 = point.second;
                center = shape_creater.CreatePoint(p1->GetXCoordinate() / 2 + p2->GetXCoordinate() / 2,
                                                   p1->GetYCoordinate() / 2 + p2->GetYCoordinate() / 2, true);
                canvas->points[center->GetId()] = center;
                canvas->midpoint[center->GetId()] = pair<Point *, Point *>(p1, p2);
                p1 = p2 = nullptr;
                return;
            }
        }
        p1 = nullptr;
    }
}

void Usr::TryConstructVertical(Canvas *canvas, double x_pos, double y_pos) {
    static Point *p{nullptr};
    static Line *l{nullptr};
    Point *pedal;
    Line *vertical;

    if (p == nullptr && l == nullptr) {
        for (auto &point: canvas->points) {
            if (point.second->IfSelect(canvas->window, x_pos, y_pos)) {
                p = point.second;
                return;
            }
        }
    } else if (p != nullptr && l == nullptr) {
        for (auto &line: canvas->lines) {
            if (line.second->IfSelect(canvas->window, x_pos, y_pos)) {
                l = line.second;
                double A, B, C;
                A = l->GetY1Coordinate() - l->GetY2Coordinate();
                B = -(l->GetX1Coordinate() - l->GetX2Coordinate());
                C = l->GetX1Coordinate() * l->GetY2Coordinate() - l->GetX2Coordinate() * l->GetY1Coordinate();

                if (fabs(A * p->GetXCoordinate() + B * p->GetYCoordinate() + C) <= PRECISION) {
                    pedal = shape_creater.CreatePoint((B * p->GetXCoordinate() - A * p->GetYCoordinate() - A) / B, -1,
                                                      true);
                } else {
                    pedal = shape_creater.CreatePoint(
                            (B * B * p->GetXCoordinate() - A * B * p->GetYCoordinate() - A * C) / (A * A + B * B),
                            (A * A * p->GetYCoordinate() - A * B * p->GetXCoordinate() - B * C) / (A * A + B * B),
                            true);
                }
                vertical = shape_creater.CreateLine(p, pedal);

                canvas->points[pedal->GetId()] = pedal;

                canvas->lines[vertical->GetId()] = vertical;

                canvas->vertical[pedal->GetId()] = pair<Point *, Line *>(p, l);
                p = nullptr, l = nullptr;
                return;
            }
        }
        p = nullptr;
    }
}

void Usr::TryConstructTangent(Canvas *canvas, double x_pos, double y_pos) {
    static Point *outside{nullptr};

    Point *p_tangent1, *p_tangent2;
    Line *tangent1, *tangent2;

    if (outside == nullptr) {
        for (auto &point: canvas->points) {
            if (point.second->IfSelect(canvas->window, x_pos, y_pos)) {
                outside = point.second;
                return;
            }
        }
    } else {
        for (auto &circle: canvas->circles) {

            auto dist_square = Point::DistSquare(circle.second->GetCenter(), outside);
            auto r_square = circle.second->GetRadius() * circle.second->GetRadius();

            if (circle.second->IfSelect(canvas->window, x_pos, y_pos) &&
                dist_square >= r_square) {
                Circle *c = circle.second;

                // on arc
                if (sqrt(dist_square) - sqrt(r_square) <= PRECISION) {
                    double A, B;
                    Line l(c->GetCenter(), outside);
                    A = l.GetY1Coordinate() - l.GetY2Coordinate();
                    B = -(l.GetX1Coordinate() - l.GetX2Coordinate());

                    p_tangent1 = shape_creater.CreatePoint(
                            (B * outside->GetXCoordinate() - A * outside->GetYCoordinate() - A) / B, -1, true);
                    tangent1 = shape_creater.CreateLine(p_tangent1, outside);

                    canvas->points[p_tangent1->GetId()] = p_tangent1;
                    canvas->lines[tangent1->GetId()] = tangent1;

                    canvas->tangent[pair<univ_id_t, univ_id_t>(p_tangent1->GetId(), 0)]
                            = pair<Point *, Circle *>(outside, c);
                } else {
                    // length from outside to p_tangent
                    double length = sqrt(dist_square - r_square);
                    // vector: outside to c
                    ImVec2 v((c->GetCenterXCoordinate() - outside->GetXCoordinate()) / sqrt(dist_square),
                             (c->GetCenterYCoordinate() - outside->GetYCoordinate()) / sqrt(dist_square));
                    // angle between tangent and c
                    auto angle = asin(sqrt(r_square) / sqrt(dist_square));

                    double x1, y1, x2, y2;
                    x1 = v.x * cos(angle) - v.y * sin(angle), y1 = v.x * sin(angle) + v.y * cos(angle);
                    x2 = v.x * cos(-angle) - v.y * sin(-angle), y2 = v.x * sin(-angle) + v.y * cos(-angle);
                    p_tangent1 = shape_creater.CreatePoint(x1 * length + outside->GetXCoordinate(),
                                                           y1 * length + outside->GetYCoordinate(), true);
                    p_tangent2 = shape_creater.CreatePoint(x2 * length + outside->GetXCoordinate(),
                                                           y2 * length + outside->GetYCoordinate(), true);

                    tangent1 = shape_creater.CreateLine(p_tangent1, outside);
                    tangent2 = shape_creater.CreateLine(p_tangent2, outside);

                    canvas->points[p_tangent1->GetId()] = p_tangent1;
                    canvas->points[p_tangent2->GetId()] = p_tangent2;

                    canvas->lines[tangent1->GetId()] = tangent1;
                    canvas->lines[tangent2->GetId()] = tangent2;

                    canvas->tangent[pair<univ_id_t, univ_id_t>(p_tangent1->GetId(), p_tangent2->GetId())]
                            = pair<Point *, Circle *>(outside, c);
                }

                outside = nullptr;
                return;
            }
        }
        outside = nullptr;
    }
}

/**
* Transform
*/

void Usr::TryTransformReflectAboutPoint(Canvas *canvas, double x_pos, double y_pos) {
    static Point *pivot{nullptr};
    Shape *reflected;
    /**
     * only implement circle reflect about point
     */
    if (pivot == nullptr) {
        for (auto &point: canvas->points) {
            if (point.second->IfSelect(canvas->window, x_pos, y_pos)) {
                pivot = point.second;
                return;
            }
        }
    } else {
        for (auto &circle: canvas->circles) {
            if (circle.second->IfSelect(canvas->window, x_pos, y_pos)) {
                Point *center_reflected = shape_creater.CreatePoint(
                        2 * pivot->GetXCoordinate() - circle.second->GetCenterXCoordinate(),
                        2 * pivot->GetYCoordinate() - circle.second->GetCenterYCoordinate(), true);

                Point *on_arc_reflected = shape_creater.CreatePoint(
                        2 * pivot->GetXCoordinate() - circle.second->GetPointOnArc()->GetXCoordinate(),
                        2 * pivot->GetYCoordinate() - circle.second->GetPointOnArc()->GetYCoordinate(), true);

                reflected = shape_creater.CreateCircle(center_reflected, on_arc_reflected);

                canvas->circles[reflected->GetId()] = (Circle *) reflected;
                canvas->reflect_about_point[reflected] = pair<Point *, Circle *>(pivot, circle.second);

                pivot = nullptr;
                return;
            }
        }
        pivot = nullptr;
    }
}