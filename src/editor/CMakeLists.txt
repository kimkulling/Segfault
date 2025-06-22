add_executable(segfault_editor
    main.cpp
)

target_link_libraries(segfault_editor
    PRIVATE
        segfault_runtime
        SDL2::SDL2
        glm::glm
        GLEW::GLEW
)
