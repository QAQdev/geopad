#include "include/Canvas.h"
#include <iostream>

unsigned Canvas::canvas_counter = 0;
unsigned Canvas::cur_canvas_idx = 0;
map<unsigned,Canvas*> Canvas::canvas_map;

map<univ_id_t, Point *> Canvas::points;
map<univ_id_t, Circle *> Canvas::circles;
map<univ_id_t, Line *> Canvas::lines;
map<univ_id_t, Rect *> Canvas::rects;
map<univ_id_t, Polygon *> Canvas::polygons;

map<univ_id_t,pair<Point*,Point*>> Canvas::midpoint;
map<univ_id_t,pair<Point*,Line*>> Canvas::vertical;
map<pair<univ_id_t,univ_id_t>,pair<Point*,Circle*>> Canvas::tangent;
map<Shape*,pair<Point*,Circle*>> Canvas::reflect_about_point;

bool Canvas::show_canvas_color = false;
bool Canvas::show_save = false;
bool Canvas::show_save_confirm = false;
bool Canvas::show_open = false;
bool Canvas::show_right_click_edit_color = false;
bool Canvas::show_right_click_edit_type = false;

Canvas::Canvas()
    :title("Geogebra"),
    width(1500),
    height(1500) {
    canvas_map[canvas_counter] = this;
    canvas_counter++;
}

Canvas::Canvas(std::string &_title,int _width,int _height)
    :title(_title),
    width(_width),
    height(_height) {
    canvas_map[canvas_counter] = this;
    canvas_counter++;
}

Canvas::~Canvas() {
    canvas_map.erase(canvas_counter);
    canvas_counter--;
}

bool Canvas::InitWindow() {

    glfwSetErrorCallback(GLFWErrorCallback);
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create window with graphics context
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) return false;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Callbacks
    glfwSetFramebufferSizeCallback(window, FrameResizeCallback);
    glfwSetCursorPosCallback(window, MouseMotionCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetKeyCallback(window, KeyBoardCallback);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    float highDPIScaleFactor = 1.0;

    io.Fonts->AddFontFromFileTTF(
            "../JetBrainsMono-ExtraLight.ttf",
            32.0f * highDPIScaleFactor,
            nullptr,
            nullptr
    );

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    return true;
}

void Canvas::MainLoop() {
    bool show_demo_window = true;

    while (!glfwWindowShouldClose(window)) {

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /**
         * main menu
         */
        static ImVec2 bar_size;
        DrawMainMenu(bar_size);

        /**
        * draw side bar
        */
        DrawSideBar(bar_size);

        /**
         * draw setting - canvas color
         */
        DrawSettings(clear_color);

        /**
         * draw file open
         */
        if (show_open) {
            DrawFileOpen();
        }

        /**
         * draw file saving
         */
        if (show_save) {
            DrawFileSave();
        }

        /**
         * when deleting, single click pops 'delete'
         */
        DrawDeleteButton();

        /**
         * single right click pops menu
         */
        DrawRightClickMenu();

        /**
         * rendering
         */
        ImGui::Render();
        glClearColor(clear_color.x * clear_color.w,
                     clear_color.y * clear_color.w,
                     clear_color.z * clear_color.w,
                     clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /**
         * draw all shapes
         */
        for (auto &circle: circles) {
            circle.second->Draw();
        }

        for (auto &point: points) {
            point.second->Draw();
        }

        for (auto &line: lines) {
            line.second->Draw();
        }

        for (auto &rect: rects) {
            rect.second->Draw();
        }

        for (auto &polygon: polygons) {
            polygon.second->Draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Canvas::DestroyWindow() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

/**
 * callbacks
 */
void Canvas::GLFWErrorCallback(int error, const char * description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void Canvas::FrameResizeCallback(GLFWwindow *window, int w, int h) {

    GLfloat aspectRatio;
    // 防止被0所除
    if (h == 0) {
        h = 1;
    }
    // 设置视口为窗口的大小
    glViewport(0, 0, w, h);
//    // 选择投影矩阵，并重置坐标系统
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
////     计算窗口的纵横比（像素比）
//    aspectRatio = (GLfloat) w / (GLfloat) h;
//    // 定义裁剪区域（根据窗口的纵横比，并使用正投影）
//    if (w <= h) {// 宽 < 高
//        glOrtho(-1.0, 1.0, -1 / aspectRatio, 1 / aspectRatio, 1.0, -1.0);
//    } else {// 宽 > 高
//        glOrtho(-1 * aspectRatio, 1 * aspectRatio, -1, 1, 1.0, -1.0);
//    }
//    // 选择模型视图矩阵，并重置坐标系统
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//
//    canvas->width = w;
//    canvas->height = h;
//    glFlush();
}

void Canvas::MouseMotionCallback (GLFWwindow * window, double x, double y) {
    Canvas *canvas = canvas_map[cur_canvas_idx];
    if (UsrMode::move && Status::is_dragged) {
        canvas->usr.DragPoint(canvas, x, y);
    }
}

void Canvas::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    double x_pos, y_pos;
    int win_w, win_h;
    glfwGetCursorPos(window, &x_pos, &y_pos);
    glfwGetWindowSize(window, &win_w, &win_h);

    ImGuiIO &io = ImGui::GetIO();
    Canvas *canvas = canvas_map[cur_canvas_idx];

    if (!io.WantCaptureMouse) {
        if (action == GLFW_PRESS) {
            switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT:
                    if (UsrMode::move) {
                        Status::is_dragged = true;
                    } else if (UsrMode::delete_) {
                        Status::is_selected = canvas->usr.SelectShape(canvas, x_pos, y_pos);
                    } else if (UsrMode::circle) {
                        canvas->usr.TryCreateCircle(canvas, x_pos, y_pos);
                    } else if (UsrMode::line) {
                        canvas->usr.TryCreateLine(canvas, x_pos, y_pos);
                    } else if (UsrMode::rect) {
                        canvas->usr.TryCreateRect(canvas, x_pos, y_pos);
                    } else if (UsrMode::point) {
                        canvas->usr.TryCreatePoint(canvas, x_pos, y_pos);
                    } else if (UsrMode::polygon) {
                        canvas->usr.TryCreatePolygon(canvas, x_pos, y_pos);
                    } else if (UsrMode::midpoint) {
                        canvas->usr.TryConstructCenter(canvas, x_pos, y_pos);
                    } else if (UsrMode::vertical) {
                        canvas->usr.TryConstructVertical(canvas, x_pos, y_pos);
                    } else if (UsrMode::tangent) {
                        canvas->usr.TryConstructTangent(canvas, x_pos, y_pos);
                    } else if (UsrMode::reflect_about_point) {
                        canvas->usr.TryTransformReflectAboutPoint(canvas, x_pos, y_pos);
                    }
                    break;
                case GLFW_MOUSE_BUTTON_RIGHT:
                    Status::is_selected = canvas->usr.SelectShape(canvas, x_pos, y_pos);
                    break;
                default:
                    break;
            }
        }

        if (action == GLFW_RELEASE) {
            switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT:
                    Status::is_dragged = false;
                    break;
                default:
                    break;
            }
        }
        glFlush();
    }
}

void Canvas::KeyBoardCallback(GLFWwindow *,int key, int scancode, int action, int mods) {
    ImGuiIO &io = ImGui::GetIO();

    if (!io.WantCaptureKeyboard) {
        if (action == GLFW_PRESS) {
            switch (mods) {
                case GLFW_MOD_CONTROL:
                    if (key == GLFW_KEY_S) {
                        show_save = true;
                    } else if (key == GLFW_KEY_O) {
                        show_open = true;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

/**
 * Draw widgets
 */

void Canvas::DrawFileSave() {
    ImGui::Begin("Saving file", &show_save, ImGuiWindowFlags_AlwaysAutoResize);
    static char buf[64] = "";
    ImGui::Text("Type in your working directory:");
    if (!ImGui::Button("Done!") || *buf == '\0') {
        ImGui::SameLine();
        ImGui::InputText(" ", buf, 64, ImGuiInputTextFlags_CharsNoBlank);
    } else show_save_confirm = true;

    if (show_save_confirm) {
        if (!ImGui::IsPopupOpen("Save?"))
            ImGui::OpenPopup("Save?");
        if (ImGui::BeginPopupModal("Save?", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Save change to the following working directory?");
            float item_height = ImGui::GetTextLineHeightWithSpacing();
            if (ImGui::BeginChildFrame(ImGui::GetID("frame"), ImVec2(-FLT_MIN, 6.25f * item_height))) {
                ImGui::Text("%s", const_cast<char *>(buf));
                ImGui::EndChildFrame();
            }

            ImVec2 button_size(ImGui::GetFontSize() * 7.0f, 0.0f);
            if (ImGui::Button("Yes", button_size)) {
                IO io(buf);
                if (!io.WriteCanvasData(this)) {

                }
                show_save = false;
                show_save_confirm = false;
                memset(buf, 0, 64);
            }
            ImGui::SameLine();
            if (ImGui::Button("No", button_size)) {
                show_save = false;
                show_save_confirm = false;
                memset(buf, 0, 64);
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", button_size)) {
                show_save_confirm = false;
                memset(buf, 0, 64);
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void Canvas::DrawFileOpen() {
    ImGui::Begin("Opening file", &show_open, ImGuiWindowFlags_AlwaysAutoResize);
    static char buf[64] = "";
    ImGui::Text("Type in your working directory:");
    if (!ImGui::Button("Done!") || *buf == '\0') {
        ImGui::SameLine();
        ImGui::InputText(" ", buf, 64, ImGuiInputTextFlags_CharsNoBlank);
    } else {
        IO io(buf);
        io.ReadCanvasData(this);
        show_open = false;
        memset(buf, 0, 64);
    }
    ImGui::End();
}

void Canvas::DrawSideBar(const ImVec2&bar_size) const {

    ImGui::SetNextWindowPos(ImVec2(0, 0 + bar_size.y));
    ImGui::SetNextWindowSize(ImVec2(30.f * 8, height), 1);
    ImGui::Begin("Operation", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    static int selected = -1;
    if (ImGui::TreeNode("User Mode")) {
        if (ImGui::Selectable("Move", selected == 0)) {
            UsrMode::SetMode(UsrMode::move);
            Status::InitAllStatus();
            selected = 0;
        }
        if (ImGui::Selectable("Delete", selected == 1)) {
            UsrMode::SetMode(UsrMode::delete_);
            Status::InitAllStatus();
            selected = 1;
        }
        if (ImGui::Selectable("Point", selected == 2)) {
            UsrMode::SetMode(UsrMode::point);
            Status::InitAllStatus();
            selected = 2;
        }
        if (ImGui::Selectable("Circle", selected == 3)) {
            UsrMode::SetMode(UsrMode::circle);
            Status::InitAllStatus();
            selected = 3;
        }
        if (ImGui::Selectable("Rect", selected == 4)) {
            UsrMode::SetMode(UsrMode::rect);
            Status::InitAllStatus();
            selected = 4;
        }
        if (ImGui::Selectable("Line", selected == 5)) {
            UsrMode::SetMode(UsrMode::line);
            Status::InitAllStatus();
            selected = 5;
        }
        if (ImGui::Selectable("Polygon", selected == 6)) {
            UsrMode::SetMode(UsrMode::polygon);
            Status::InitAllStatus();
            selected = 6;
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Construct")) {
        if (ImGui::Selectable("Center", selected == 7)) {
            UsrMode::SetMode(UsrMode::midpoint);
            Status::InitAllStatus();
            selected = 7;
        }
        if (ImGui::Selectable("Vertical", selected == 8)) {
            UsrMode::SetMode(UsrMode::vertical);
            Status::InitAllStatus();
            selected = 8;
        }
        if (ImGui::Selectable("Tangent", selected == 9)) {
            UsrMode::SetMode(UsrMode::tangent);
            Status::InitAllStatus();
            selected = 9;
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Transform")) {
        if (ImGui::Selectable("Reflect\nabout point", selected == 10)) {
            UsrMode::SetMode(UsrMode::reflect_about_point);
            Status::InitAllStatus();
            selected = 10;
        }

        ImGui::TreePop();
    }

    ImGui::End();
}

void Canvas::DrawMainMenu(ImVec2 &bar_size) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "Ctrl-O")) {
                show_open = true;
            }
            if (ImGui::MenuItem("Save", "Ctrl-S")) {
                show_save = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings")) {
            if (ImGui::MenuItem("Canvas color")) {
                show_canvas_color = true;
            }
            ImGui::EndMenu();
        }
        bar_size = ImGui::GetWindowSize();
        ImGui::EndMainMenuBar();
    }
}

void Canvas::DrawSettings(ImVec4&clear_color) {
    if (show_canvas_color) {
        ImGui::Begin("Canvas color", &show_canvas_color,
                     ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::ColorEdit3("Background color", (float *) &clear_color); // Edit 3 floats representing a color
        ImGui::End();
    }
}

void Canvas::DrawDeleteButton() {
    static double mx, my;
    if (UsrMode::delete_) {
        if (!Status::is_selected) {
            glfwGetCursorPos(window, &mx, &my);
        } else {
            ImGui::SetNextWindowPos(ImVec2(mx, my));
            ImGui::Begin(
                    " ", nullptr,
                    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

            if (ImGui::Button("Delete")) {
                usr.TryDelete(this);
                Status::is_selected = false;
            }
            ImGui::End();
        }
    }
}

void Canvas::DrawRightClickMenu() {
    static double mx, my;
    if (!UsrMode::delete_) {
        if (!Status::is_selected) {
            glfwGetCursorPos(window, &mx, &my);
        } else {
            ImGui::SetNextWindowPos(ImVec2(mx, my));
            ImGui::Begin(
                    " ", nullptr,
                    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

            if (ImGui::MenuItem("Color")) {
                show_right_click_edit_color = true;
                Status::is_selected = false;
            }
            if (ImGui::MenuItem("Type")) {
                show_right_click_edit_type = true;
                Status::is_selected = false;
            }
            ImGui::End();
        }
    }

    static ImVec4 shape_color;
    if (show_right_click_edit_color) {

        ImGui::Begin("Edit Color", &show_right_click_edit_color, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::ColorEdit3(" ", (float *) &shape_color);
        usr.to_be_edited->SetRGB(shape_color.x, shape_color.y, shape_color.z);
        ImGui::End();
    }

    static Type shape_type;
    if (show_right_click_edit_type) {
        ImGui::SetNextWindowSize(ImVec2(250.0f, 150.0f), 1);
        ImGui::Begin("Edit Type", &show_right_click_edit_type, ImGuiWindowFlags_NoResize);
        if (ImGui::Selectable("Stipple", (int) shape_type == (int) Type::DOTTED)) {
            usr.to_be_edited->SetType(shape_type);
            shape_type = Type::DOTTED;
        }
        if (ImGui::Selectable("Solid", (int) shape_type == (int) Type::SOLID)) {
            usr.to_be_edited->SetType(shape_type);
            shape_type = Type::SOLID;
        }
        ImGui::End();
    }
}