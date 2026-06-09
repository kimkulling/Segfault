#include "vulkandevice.h"

namespace segfault::renderer {

    bool QueueFamilyIndices::isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value() &&
               computeFamily.has_value() && transferFamily.has_value();
    }

    bool QueueFamilyIndices::isGraphicsComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

} // namespace segfault::renderer
