#include "core/segfault.h"
#include "application/app.h"

using namespace segfault::application;
using namespace segfault::core;

int main(int argc, char* argv[]) {
    App myApp;
    uint32_t width = 800;
    uint32_t height = 600;
    if (!myApp.init("hello_world", Rect(50, 50, width, height), "hello, world!", false)) {
        return -1;
    }
    
    while (myApp.mainloop()) {
        myApp.drawFrame();
    }
    myApp.shutdown();

    return 0;
}
