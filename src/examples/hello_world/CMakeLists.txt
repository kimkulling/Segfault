add_executable(hello_world hello_world.cpp)
target_link_libraries(hello_world example_common)
add_dependencies(hello_world compile_all_assets)

set_target_properties(hello_world PROPERTIES FOLDER examples )
