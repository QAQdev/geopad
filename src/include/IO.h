#ifndef GEOPAD_IO_H
#define GEOPAD_IO_H

#include <fstream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ShapeCreater.h"
#include "json/json.h"

#ifdef _WIN32
#include <direct.h>		//for mkdir rmdir
#include <io.h>			//for access
#elif __linux__
#include <unistd.h>		//for mkdir rmdir
#include <sys/stat.h>	//for access
#include <dirent.h>		//for DIR remove
#endif

#ifdef _WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#elif __linux__
#define ACCESS access
#define MKDIR(a) mkdir((a),0755)
#endif

class Canvas;

class IO {
private:
    string path;
    static string cur_working_path;
public:
    IO() : path("../geofiles/sample/") {}

    IO(const string &_path);

    ~IO() = default;

public:
    bool WriteCanvasData(const Canvas *);

    bool ReadCanvasData(Canvas *);

private:
    bool MkDir();

};


#endif //GEOPAD_IO_H
