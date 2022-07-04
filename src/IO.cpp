#include "include/Canvas.h"
#include "include/IO.h"

string IO::cur_working_path;

IO::IO(const string &_path) {
    path = "../geofiles/" + _path + '/';
}

bool IO::WriteCanvasData(const Canvas*canvas) {
    Json::Value root, sub_root;
    Json::Value array(Json::arrayValue), sub_array(Json::arrayValue);

    if (!MkDir()) return false;

    // points
    auto idx = 0;
    for (auto &point: canvas->points) {
        sub_root["id"] = point.second->GetId();
        sub_root["name"] = point.second->text;
        sub_root["x"] = point.second->GetXCoordinate();
        sub_root["y"] = point.second->GetYCoordinate();
        sub_root["drawType"] = static_cast<unsigned>(point.second->type);
        sub_root["r"] = point.second->r;
        sub_root["g"] = point.second->g;
        sub_root["b"] = point.second->b;
        sub_root["fill"] = point.second->set_fill;
        array[idx] = sub_root;
        idx++;
    }
    root["Items"] = array;
    root["Type"] = "points";

    Json::StyledWriter styled_writer;
    ofstream os;
    os.open(path + "points.json", std::ios::out);
    if (!os.is_open()) return false;
    os << styled_writer.write(root);
    os.close();

    // circles
    root.clear();
    sub_root.clear();
    array.clear();
    idx = 0;
    for (auto &circle: canvas->circles) {
        sub_root["center"] = circle.second->GetCenter()->GetId();
        sub_root["onArc"] = circle.second->GetPointOnArc()->GetId();
        sub_root["drawType"] = static_cast<unsigned>(circle.second->type);
        sub_root["r"] = circle.second->r;
        sub_root["g"] = circle.second->g;
        sub_root["b"] = circle.second->b;
        sub_root["fill"] = circle.second->set_fill;
        array[idx] = sub_root;
        idx++;
    }
    root["Items"] = array;
    root["Type"] = "circles";

    os.open(path + "circles.json", std::ios::out);
    if (!os.is_open()) return false;
    os << styled_writer.write(root);
    os.close();

    // polygons
    root.clear();
    sub_root.clear();
    array.clear();
    idx = 0;
    for (auto &polygon: canvas->polygons) {
        auto i = 0;
        for (auto &point: polygon.second->GetPointsOnEdge()) {
            sub_array[i] = point->GetId();
            i++;
        }
        sub_root["pointOnEdge"] = sub_array;
        sub_root["drawType"] = static_cast<unsigned>(polygon.second->type);
        sub_root["r"] = polygon.second->r;
        sub_root["g"] = polygon.second->g;
        sub_root["b"] = polygon.second->b;
        sub_root["fill"] = polygon.second->set_fill;
        array[idx] = sub_root;
        idx++;
    }
    root["Items"] = array;
    root["Type"] = "polygons";

    os.open(path + "polygons.json", std::ios::out);
    if (!os.is_open()) return false;
    os << styled_writer.write(root);
    os.close();

    // lines
    root.clear();
    sub_root.clear();
    array.clear();
    idx = 0;
    for (auto &line: canvas->lines) {
        sub_root["dst"] = line.second->GetDst()->GetId();
        sub_root["src"] = line.second->GetSrc()->GetId();
        sub_root["drawType"] = static_cast<unsigned>(line.second->type);
        sub_root["r"] = line.second->r;
        sub_root["g"] = line.second->g;
        sub_root["b"] = line.second->b;
        sub_root["fill"] = line.second->set_fill;
        array[idx] = sub_root;
        idx++;
    }
    root["Items"] = array;
    root["Type"] = "line";

    os.open(path + "lines.json", std::ios::out);
    if (!os.is_open()) return false;
    os << styled_writer.write(root);
    os.close();

    // rects
    root.clear();
    sub_root.clear();
    array.clear();
    idx = 0;
    for (auto &rect: canvas->rects) {
        sub_root["topLeft"] = rect.second->GetTopLeft()->GetId();
        sub_root["lowerRight"] = rect.second->GetLowerRight()->GetId();
        sub_root["drawType"] = static_cast<unsigned>(rect.second->type);
        sub_root["r"] = rect.second->r;
        sub_root["g"] = rect.second->g;
        sub_root["b"] = rect.second->b;
        sub_root["fill"] = rect.second->set_fill;
        array[idx] = sub_root;
        idx++;
    }
    root["Items"] = array;
    root["Type"] = "rects";

    os.open(path + "rects.json", std::ios::out);
    if (!os.is_open()) return false;
    os << styled_writer.write(root);
    os.close();

    // geometry relations
    // construct: midpoint
    root.clear();
    sub_root.clear();
    array.clear();
    idx = 0;
    for (auto &it: canvas->midpoint) {
        sub_root["point1"] = it.second.first->GetId();
        sub_root["point2"] = it.second.second->GetId();
        sub_root["midpoint"] = it.first;
        array[idx] = sub_root;
        idx++;
    }

    root["Items"] = array;
    root["Construct"] = "midpoint";

    os.open(path + "construct_midpoint.json", std::ios::out);
    if (!os.is_open()) return false;
    os << styled_writer.write(root);
    os.close();

    // construct: vertical
    root.clear();
    sub_root.clear();
    array.clear();
    idx = 0;
    for (auto &it: canvas->vertical) {
        sub_root["point"] = it.second.first->GetId();
        sub_root["lineSrc"] = it.second.second->GetSrc()->GetId();
        sub_root["lineDst"] = it.second.second->GetDst()->GetId();
        sub_root["pedal"] = it.first;
        array[idx] = sub_root;
        idx++;
    }

    root["Items"] = array;
    root["Construct"] = "vertical";

    os.open(path + "construct_vertical.json", std::ios::out);
    if (!os.is_open()) return false;
    os << styled_writer.write(root);
    os.close();

    // construct: tangent
    root.clear();
    sub_root.clear();
    array.clear();
    idx = 0;
    for (auto &it: canvas->tangent) {
        sub_root["pTangent1"] = it.first.first;
        sub_root["pTangent2"] = it.first.second;
        sub_root["outsidePoint"] = it.second.first->GetId();
        sub_root["circleCenter"] = it.second.second->GetCenter()->GetId();
        sub_root["circleOnArc"] = it.second.second->GetPointOnArc()->GetId();
        array[idx] = sub_root;
        idx++;
    }

    root["Items"] = array;
    root["Construct"] = "tangent";

    os.open(path + "construct_tangent.json", std::ios::out);
    if (!os.is_open()) return false;
    os << styled_writer.write(root);
    os.close();

    // transform: reflect about point
    root.clear();
    sub_root.clear();
    array.clear();
    idx = 0;
    for (auto &it: canvas->reflect_about_point) {
        sub_root["shapeType"] = static_cast<int>(it.first->GetShapeType());
        sub_root["pivot"] = it.second.first->GetId();
        sub_root["originShapePoint1"] = it.second.second->GetCenter()->GetId();
        sub_root["originShapePoint2"] = it.second.second->GetPointOnArc()->GetId();
//        sub_root["reShapePoint1"] = dynamic_cast<Circle*>(it.first)->GetCenter()->GetId();
//        sub_root["reShapePoint2"] = dynamic_cast<Circle*>(it.first)->GetPointOnArc()->GetId();
        array[idx] = sub_root;
        idx++;
    }

    root["Items"] = array;
    root["Transform"] = "reflect-about-point";

    os.open(path + "transform_reflect_about_point.json", std::ios::out);
    if (!os.is_open()) return false;
    os << styled_writer.write(root);
    os.close();

    IO::cur_working_path = path;
    return true;
}

bool IO::ReadCanvasData(Canvas*canvas) {
    Json::Reader reader;
    Json::Value root;
    Json::Value array(Json::arrayValue), sub_array(Json::arrayValue);
    auto idx = 0;

    if (path == cur_working_path) return true;

    ifstream is(path + "points.json", ios::binary);
    if (!is.is_open()) return false;

    map<univ_id_t, Point *> point_mapping;
    if (reader.parse(is, root)) {
        for (idx = 0; idx < root["Items"].size(); idx++) {
            auto x = root["Items"][idx]["x"].asDouble(), y = root["Items"][idx]["y"].asDouble();
            auto point = canvas->usr.shape_creater.CreatePoint(x, y);
            point->set_fill = root["Items"][idx]["fill"].asBool();
            point->SetName(root["Items"][idx]["name"].asString());
            point->SetType(static_cast<Type>(root["Items"][idx]["drawType"].asInt()));
            point->SetRGB(root["Items"][idx]["r"].asDouble(),
                          root["Items"][idx]["g"].asDouble(),
                          root["Items"][idx]["b"].asDouble());
            point_mapping[root["Items"][idx]["id"].asUInt64()] = point;
            canvas->points[point->GetId()] = point;
        }
    }

    is.close();

    map<pair<univ_id_t, univ_id_t>, Circle *> circle_mapping;
    is.open(path + "circles.json", ios::binary);
    if (!is.is_open()) return false;

    if (reader.parse(is, root)) {
        for (idx = 0; idx < root["Items"].size(); idx++) {
            auto id1 = root["Items"][idx]["center"].asUInt64(), id2 = root["Items"][idx]["onArc"].asUInt64();
            if (point_mapping[id1] == nullptr || point_mapping[id2] == nullptr) continue;
            auto circle = canvas->usr.shape_creater.CreateCircle(point_mapping[id1], point_mapping[id2]);
            circle->SetType(static_cast<Type>(root["Items"][idx]["drawType"].asInt()));
            circle->SetName(root["Items"][idx]["name"].asString());
            circle->SetRGB(root["Items"][idx]["r"].asDouble(),
                           root["Items"][idx]["g"].asDouble(),
                           root["Items"][idx]["b"].asDouble());
            canvas->circles[circle->GetId()] = circle;
            circle_mapping[pair<univ_id_t, univ_id_t>(id1, id2)] = circle;
        }
    }
    is.close();

    is.open(path + "polygons.json", ios::binary);
    if (!is.is_open()) return false;

    vector<Point *> on_edge;
    if (reader.parse(is, root)) {
        for (idx = 0; idx < root["Items"].size(); idx++) {
            for (auto i = 0; i < root["Items"][idx]["pointOnEdge"].size(); i++) {
                on_edge.push_back(point_mapping[root["Items"][idx]["pointOnEdge"][i].asUInt64()]);
            }
            auto polygon = canvas->usr.shape_creater.CreatePolygon(on_edge);
            polygon->SetType(static_cast<Type>(root["Items"][idx]["drawType"].asInt()));
            polygon->SetName(root["Items"][idx]["name"].asString());
            polygon->SetRGB(root["Items"][idx]["r"].asDouble(),
                            root["Items"][idx]["g"].asDouble(),
                            root["Items"][idx]["b"].asDouble());
            canvas->polygons[polygon->GetId()] = polygon;
        }
    }
    is.close();

    is.open(path + "rects.json", ios::binary);
    if (!is.is_open()) return false;

    if (reader.parse(is, root)) {
        for (idx = 0; idx < root["Items"].size(); idx++) {
            auto id1 = root["Items"][idx]["topLeft"].asUInt64(), id2 = root["Items"][idx]["lowerRight"].asUInt64();
            auto rect = canvas->usr.shape_creater.CreateRect(point_mapping[id1], point_mapping[id2]);
            rect->SetType(static_cast<Type>(root["Items"][idx]["drawType"].asInt()));
            rect->SetName(root["Items"][idx]["name"].asString());
            rect->SetRGB(root["Items"][idx]["r"].asDouble(),
                         root["Items"][idx]["g"].asDouble(),
                         root["Items"][idx]["b"].asDouble());
            canvas->rects[rect->GetId()] = rect;
        }
    }
    is.close();

    map<pair<univ_id_t, univ_id_t>, Line *> line_mapping;
    is.open(path + "lines.json", ios::binary);
    if (!is.is_open()) return false;

    if (reader.parse(is, root)) {
        for (idx = 0; idx < root["Items"].size(); idx++) {
            auto id1 = root["Items"][idx]["src"].asUInt64(), id2 = root["Items"][idx]["dst"].asUInt64();
            auto line = canvas->usr.shape_creater.CreateLine(point_mapping[id1], point_mapping[id2]);
            line->SetType(static_cast<Type>(root["Items"][idx]["drawType"].asInt()));
            line->SetName(root["Items"][idx]["name"].asString());
            line->SetRGB(root["Items"][idx]["r"].asDouble(),
                         root["Items"][idx]["g"].asDouble(),
                         root["Items"][idx]["b"].asDouble());
            canvas->lines[line->GetId()] = line;
            line_mapping[pair<univ_id_t, univ_id_t>(id1, id2)] = line;
        }
        is.close();

        /**
         * construct: midpoint
         */
        is.open(path + "construct_midpoint.json", ios::binary);
        if (!is.is_open()) return false;

        if (reader.parse(is, root)) {
            for (idx = 0; idx < root["Items"].size(); idx++) {
                auto midpoint = point_mapping[root["Items"][idx]["midpoint"].asUInt64()];
                auto point1 = point_mapping[root["Items"][idx]["point2"].asUInt64()],
                        point2 = point_mapping[root["Items"][idx]["point1"].asUInt64()];
                midpoint->SetNegative();
                canvas->midpoint[midpoint->GetId()] = pair<Point *, Point *>(point1, point2);
            }
        }
        is.close();

        is.open(path + "construct_vertical.json", ios::binary);
        if (!is.is_open()) return false;

        if (reader.parse(is, root)) {
            for (idx = 0; idx < root["Items"].size(); idx++) {
                auto pedal = point_mapping[root["Items"][idx]["pedal"].asUInt64()];
                auto point = point_mapping[root["Items"][idx]["point"].asUInt64()];
                auto line = line_mapping[pair<univ_id_t, univ_id_t>
                        (root["Items"][idx]["lineSrc"].asUInt64(), root["Items"][idx]["lineDst"].asUInt64())];
                pedal->SetNegative();
                canvas->vertical[pedal->GetId()] = pair<Point *, Line *>(point, line);
            }
        }
        is.close();

        is.open(path + "construct_tangent.json", ios::binary);
        if (!is.is_open()) return false;

        if (reader.parse(is, root)) {
            for (idx = 0; idx < root["Items"].size(); idx++) {
                auto p_tangent1 = point_mapping[root["Items"][idx]["pTangent1"].asUInt64()];
                Point *p_tangent2;
                auto point = point_mapping[root["Items"][idx]["outsidePoint"].asUInt64()];
                auto circle = circle_mapping[pair<univ_id_t, univ_id_t>
                        (root["Items"][idx]["circleCenter"].asUInt64(), root["Items"][idx]["circleOnArc"].asUInt64())];
                p_tangent1->SetNegative();
                if (root["Items"][idx]["pTangent2"].asUInt64() != 0) {
                    p_tangent2 = point_mapping[root["Items"][idx]["pTangent2"].asUInt64()];
                    p_tangent2->SetNegative();
                    canvas->tangent[pair<univ_id_t, univ_id_t>(p_tangent1->GetId(),
                                                               p_tangent2->GetId())]
                            = pair<Point *, Circle *>(point, circle);
                } else
                    canvas->tangent[pair<univ_id_t, univ_id_t>(p_tangent1->GetId(), 0)]
                            = pair<Point *, Circle *>(point, circle);
            }
        }
        is.close();

        is.open(path + "transform_reflect_about_point.json", ios::binary);
        if (!is.is_open()) return false;

        if (reader.parse(is, root)) {
            for (idx = 0; idx < root["Items"].size(); idx++) {
                auto pivot = point_mapping[root["Items"][idx]["pivot"].asUInt64()];
                auto origin_circle = circle_mapping[pair<univ_id_t, univ_id_t>
                        (root["Items"][idx]["originShapePoint1"].asUInt64(),
                         root["Items"][idx]["originShapePoint2"].asUInt64())];

                Point *center_reflected = canvas->usr.shape_creater.CreatePoint(
                        2 * pivot->GetXCoordinate() - origin_circle->GetCenterXCoordinate(),
                        2 * pivot->GetYCoordinate() - origin_circle->GetCenterYCoordinate(), true);

                Point *on_arc_reflected = canvas->usr.shape_creater.CreatePoint(
                        2 * pivot->GetXCoordinate() - origin_circle->GetPointOnArc()->GetXCoordinate(),
                        2 * pivot->GetYCoordinate() - origin_circle->GetPointOnArc()->GetYCoordinate(), true);

                auto reflected = canvas->usr.shape_creater.CreateCircle(center_reflected, on_arc_reflected);

                canvas->circles[reflected->GetId()] = reflected;
                canvas->reflect_about_point[reflected] = pair<Point *, Circle *>(pivot, origin_circle);
            }
        }
        is.close();

        IO::cur_working_path = path;
        return true;
    }
}

bool IO::MkDir() {
    int n_dir_len = path.length();
    if (n_dir_len <= 0) return false;
    char *p_dir_temp = new char[n_dir_len + 4];
    strcpy(p_dir_temp, path.c_str());
    p_dir_temp[n_dir_len] = '\0';

    if (p_dir_temp[n_dir_len - 1] != '\\' && p_dir_temp[n_dir_len - 1] != '/') {
        p_dir_temp[n_dir_len] = '/';
        p_dir_temp[n_dir_len + 1] = '\0';
        n_dir_len++;
    }

    // make directory
    int i = 0;
    for (i = 0; i < n_dir_len; i++) {
        if (p_dir_temp[i] == '\\' || p_dir_temp[i] == '/') {
            p_dir_temp[i] = '\0';
            auto status = ACCESS(p_dir_temp, 0);
            if (status != 0) {
                status = MKDIR(p_dir_temp);
                if (status != 0) {
                    return false;
                }
            }
            p_dir_temp[i] = '/';
        }
    }
    delete[] p_dir_temp;
    return true;
}