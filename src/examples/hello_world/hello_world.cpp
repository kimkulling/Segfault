#include "core/segfault.h"
#include "application/app.h"

using namespace segfault::application;
using namespace segfault::core;

int main(int argc, char* argv[]) {
    App myApp;
    if (!myApp.init(10, 10, 400, 300, "hello, world!", false)) {
        return -1;
    }
    
    while (myApp.run()) {

    }
    myApp.shutdown();

    return 0;
}
