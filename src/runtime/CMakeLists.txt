SET(segfault_core_src
    core/segfault.h
)

SET(segfault_renderer_src
    renderer/rendercore.h
    renderer/renderthread.cpp
    renderer/renderthread.h
    renderer/RHI.h
    renderer/RHIVulkan.cpp
)

SET(segfault_application_src
    application/app.h
    application/app.cpp
)

SOURCE_GROUP( core        FILES ${segfault_core_src} )
SOURCE_GROUP( application FILES ${segfault_application_src} )
SOURCE_GROUP( renderer    FILES ${segfault_renderer_src} )

ADD_LIBRARY(segfault_runtime SHARED
    ${segfault_core_src}
    ${segfault_renderer_src}
    ${segfault_application_src}
)

target_link_libraries(segfault_runtime PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

set_target_properties(segfault_runtime PROPERTIES FOLDER engine\\runtime )
