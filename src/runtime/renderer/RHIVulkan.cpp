#include "RHI.h"
#include "volk.h"

namespace segfault::renderer {
    
    struct RHIImpl {
        VkInstance instance;
    };

    RHI::RHI() : mImpl(nullptr) {
        // empty
    }
    
    RHI::~RHI() {

    }
    
    bool RHI::init() {
        mImpl = new RHIImpl;
        VkResult result = volkInitialize();
        if (result != VK_SUCCESS) {
            return false;
        }

        //mImpl->instance =
        return true;
    }
    
    bool RHI::shutdown() {
        volkFinalize();

        delete mImpl;
        mImpl = nullptr;

        return true;
    }
    
    void RHI::beginFrame() {
        // Code to prepare for a new frame
    }
    
    void RHI::endFrame() {
        // Code to finalize the frame
    }
    
    void RHI::submitCommandBuffer() {
        // Code to submit the command buffer for execution
    }
}
