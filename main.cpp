#include "src/include/Canvas.h"

int main() {

    Canvas canvas;
    canvas.InitWindow();
    canvas.MainLoop();
    canvas.DestroyWindow();

    return 0;
}
