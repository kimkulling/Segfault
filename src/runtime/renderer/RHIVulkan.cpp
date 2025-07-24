#include "RHI.h"
#include "volk.h"
#include "SDL_vulkan.h"

#include <vector>

namespace segfault::renderer {
    
    struct RHIImpl {
        SDL_Window *window = nullptr;
        VkInstance instance;
        VkPhysicalDevice physicalDevice;
        VkSurfaceKHR surface;
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
        mImpl = new RHIImpl;
        mImpl->window = window;
        VkResult result{};
        result = volkInitialize();
        if (result != VK_SUCCESS) {
            return false;
        }
        

        VkApplicationInfo appInfo{};
        createInstance(appInfo);        

        uint32_t extensionCount = 0;
        const char** extensions;
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
        const char **extensionNames = new const char* [extensionCount];
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensionNames);

        const VkInstanceCreateInfo createInfo = {
                VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, // sType
                nullptr,                                // pNext
                0,                                      // flags
                &appInfo,                                // pApplicationInfo
                0,                                      // enabledLayerCount
                nullptr,                                // ppEnabledLayerNames
                extensionCount,                         // enabledExtensionCount
                extensionNames,                         // ppEnabledExtensionNames
        };

        result = vkCreateInstance(&createInfo, nullptr, &mImpl->instance);
        if (result != VK_SUCCESS) {
            return false;
        }
        volkLoadInstance(mImpl->instance);
        uint32_t physicalDeviceCount;
        vkEnumeratePhysicalDevices(mImpl->instance, &physicalDeviceCount, nullptr);
        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        vkEnumeratePhysicalDevices(mImpl->instance, &physicalDeviceCount, physicalDevices.data());
        mImpl->physicalDevice = physicalDevices[0];

        SDL_Vulkan_CreateSurface(mImpl->window, mImpl->instance, &mImpl->surface);

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

} // namespace segfault::renderer
