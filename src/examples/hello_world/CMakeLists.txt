add_executable(hello_world hello_world.cpp)
target_link_libraries(hello_world segfault_runtime)

set_target_properties(hello_world PROPERTIES FOLDER examples )
