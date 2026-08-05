#include "common/examplebase.h"

using segfault::examples::ExampleBase;
using segfault::examples::ExampleConfig;

class Scene2DSample final : public ExampleBase {
public:
    Scene2DSample() :
            ExampleBase(ExampleConfig{"2d_scene_sample", "2D Scene Sample", 50, 50, 800, 600, false}) {
        // empty
    }
};

int main(int argc, char *argv[]) {
    Scene2DSample example;
    return example.run(argc, argv);
}
