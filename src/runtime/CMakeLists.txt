SET(segfault_core_src
    core/argumentparser.h
    core/argumentparser.cpp
    core/segfault.h
    core/segfaultexception.h
    core/filearchive.h
    core/ifilemanager.h
    core/genericfilemanager.h
    core/genericfilemanager.cpp
    core/tokenizer.cpp
    core/tokenizer.h
)

SET(segfault_renderer_src
    renderer/rendercore.h
    renderer/renderthread.cpp
    renderer/renderthread.h
    renderer/RHI.h
    renderer/RHIVulkan.cpp
    renderer/vulkanbuffer.cpp
    renderer/vulkanbuffer.h
    renderer/vulkandevice.cpp
    renderer/vulkandevice.h
    renderer/vulkanutils.cpp
    renderer/vulkanutils.h
    renderer/vulkantypes.h
)

SET(segfault_ai_src
    ai/behavior_tree.h
    ai/behavior_tree.cpp
)

SET(segfault_application_src
    application/app.h
    application/app.cpp
)

source_group(ai           FILES ${segfault_ai_src} )
source_group(core         FILES ${segfault_core_src} )
source_group(application  FILES ${segfault_application_src} )
source_group(renderer     FILES ${segfault_renderer_src} )

ADD_LIBRARY(segfault_runtime SHARED
    ${segfault_ai_src}    
    ${segfault_core_src}
    ${segfault_renderer_src}
    ${segfault_application_src}
)

target_link_libraries(segfault_runtime PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    volk::volk volk::volk_headers
)

set_target_properties(segfault_runtime PROPERTIES FOLDER engine\\runtime )
