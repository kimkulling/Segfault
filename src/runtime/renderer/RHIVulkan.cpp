#include "RHI.h"
#include "volk.h"
#include "SDL_vulkan.h"

namespace segfault::renderer {
    
    struct RHIImpl {
        VkInstance instance;
    };

    RHI::RHI() : mImpl(nullptr) {
        // empty
    }
    
    RHI::~RHI() {

    }
    
    static void createInstance(VkApplicationInfo &appInfo) {
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello World";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "SegFault";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
    }

    bool RHI::init(SDL_Window *window) {
        VkResult result{};
        mImpl = new RHIImpl;
        result = volkInitialize();
        if (result != VK_SUCCESS) {
            return false;
        }

        VkApplicationInfo appInfo{};
        createInstance(appInfo);

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t extensionCount = 0;
        const char** extensions;
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
        
        
        createInfo.enabledLayerCount = 0;
        result = vkCreateInstance(&createInfo, nullptr, &mImpl->instance);
        if (result != VK_SUCCESS) {
            return false;
        }

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
