add_executable(assetbaker main.cpp)
target_link_libraries(assetbaker segfault_runtime)

set_target_properties(assetbaker PROPERTIES FOLDER tools\\assetbaker )
