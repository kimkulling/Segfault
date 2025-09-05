#include "core/segfault.h"
#include "application/app.h"

using namespace segfault::application;
using namespace segfault::core;

int main(int argc, char* argv[]) {
    App myApp;
    if (!myApp.init("hello_world", 50, 50, 400, 300, "hello, world!", false)) {
        return -1;
    }
    
    while (myApp.mainloop()) {
        myApp.drawFrame();
    }
    myApp.shutdown();

    return 0;
}
