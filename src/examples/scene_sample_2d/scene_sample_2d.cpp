#include "common/examplebase.h"
#include "renderer/rendercore.h"
#include "renderer/quadgenerator.h"

using segfault::examples::ExampleBase;
using segfault::examples::ExampleConfig;
using namespace segfault::renderer;

class Scene2DSample final : public ExampleBase {
public:
    Scene2DSample() :
            ExampleBase(ExampleConfig{"2d_scene_sample", "2D Scene Sample", 50, 50, 800, 600, false}) {
        // empty
    }

protected:
    bool onSetup() override {
        QuadGenerator quadGenerator;
        Mesh mesh = quadGenerator.generate(1.0f, 1.0f, {0.0f, 0.6f, 1.0f});
        ExampleBase::getApp().getRHI()->addPrimitive(mesh);
        ExampleBase::getApp().getRHI()->setProjectionMode(ProjectionMode::Orthographic);

        return ExampleBase::getApp().initRenderer();
    }
};

int main(int argc, char *argv[]) {
    Scene2DSample example;
    return example.run(argc, argv);
}
