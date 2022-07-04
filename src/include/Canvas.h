#ifndef GEOPAD_CANVAS_H
#define GEOPAD_CANVAS_H

#include <map>
#include <string>
#include <algorithm>

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Config.h"
#include "ShapeCreater.h"
#include "IO.h"
#include "Usr.h"

using namespace std;

class Canvas {
private:
    GLFWwindow *window;
    std::string title;
    int width, height;
    const string glsl_version = "#version 150";

    ImVec4 clear_color = ImVec4(0.81f, 0.88f, 0.92f, 1.0f);

    friend class Usr;

    Usr usr;

    friend class IO;

    static unsigned canvas_counter;
    static unsigned cur_canvas_idx;
    static map<unsigned, Canvas *> canvas_map;

    /**
     * all shapes;
     */
    static map<univ_id_t, Point *> points;
    static map<univ_id_t, Circle *> circles;
    static map<univ_id_t, Line *> lines;
    static map<univ_id_t, Rect *> rects;
    static map<univ_id_t, Polygon *> polygons;

    /**
     * geometry relation
     */
    static map<univ_id_t, pair<Point *, Point *>> midpoint; // key: midpoint
    static map<univ_id_t,pair<Point*,Line*>> vertical; // key: pedal
    static map<pair<univ_id_t,univ_id_t>,pair<Point*,Circle*>> tangent; // key: a pair of tangent points
    static map<Shape*,pair<Point*,Circle*>> reflect_about_point; //key: reflected shape

    static bool show_canvas_color;
    static bool show_save, show_save_confirm;
    static bool show_open;
    static bool show_right_click_edit_color;
    static bool show_right_click_edit_type;

public:
    Canvas();

    Canvas(std::string &, int, int);

    ~Canvas();

    bool InitWindow();

    void MainLoop();

    void DestroyWindow();

private:
    void DrawFileSave();

    void DrawFileOpen();

    void DrawSideBar(const ImVec2&) const;

    void DrawMainMenu(ImVec2&);

    void DrawSettings(ImVec4&);

    void DrawDeleteButton();

    void DrawRightClickMenu();

private:

    /**
     * callbacks
     */
    static void GLFWErrorCallback(int, const char *);

    static void FrameResizeCallback(GLFWwindow *, int, int);

    static void MouseMotionCallback(GLFWwindow *, double, double);

    static void MouseButtonCallback(GLFWwindow *, int, int, int);

    static void KeyBoardCallback(GLFWwindow *, int, int, int, int);
};


#endif //GEOPAD_CANVAS_H
