#ifndef GEOGEBRA_SHAPE_H
#define GEOGEBRA_SHAPE_H

#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <chrono>
#include <functional>

/**
 * @brief abstract class
 */

enum class Type {
    SOLID, DOTTED
};

enum class ShapeType {
    NONE, POINT, CIRCLE, RECT, LINE, POLYGON
};

typedef unsigned long long univ_id_t;

class HashId {
private:
    inline static std::string CurrentTimeFormatString() {
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        auto sec = std::chrono::duration_cast<std::chrono::seconds>(now_ms);
        int32_t milltime = now_ms.count() % 1000;

        std::time_t time = sec.count();
        struct tm cur_time;
        // localtime_r(&time, &cur_time); // use `localtime_r` on unix
        cur_time = *localtime(&time);

        char buffer[64];
        sprintf(buffer, "%4d-%02d-%02d-%02d:%02d:%02d.%03d", cur_time.tm_year + 1900, cur_time.tm_mon + 1,
                cur_time.tm_mday, cur_time.tm_hour, cur_time.tm_min, cur_time.tm_sec, milltime);
        return std::string(buffer);
    }

public:
    static univ_id_t UnivId() {
        static unsigned salt = 0;
        std::hash<std::string> hash;
        std::string s = CurrentTimeFormatString() + "salt=" + std::to_string(salt++);
        auto val = hash.operator()(s);
        return val;
    }
};

class IO;

class Shape {
protected:
    friend class IO;

    double r, g, b;
    univ_id_t id;
    Type type;
    bool set_fill;
    std::string text;
public:
    Shape();

    Shape(double _r, double _g, double _b) : r(_r), g(_g), b(_b) {
        type = Type::DOTTED;
        set_fill = false;
        glLineWidth(2.7);
        glPointSize(12.0f);
    }

    virtual ~Shape() = default;

    virtual void Draw() const {}

    virtual unsigned int GetId() const { return -1; }

    virtual void SetType(Type) {}

    virtual void SetRGB(double, double, double) {}

    virtual void SetFill() {}

    virtual void SetName(const std::string &) {}

    virtual ShapeType GetShapeType() { return ShapeType::NONE; }

    virtual bool IfSelect(GLFWwindow *, double, double) { return false; }
};

#endif //GEOGEBRA_SHAPE_H
