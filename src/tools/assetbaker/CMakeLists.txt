add_executable(assetbaker main.cpp)
target_link_libraries(assetbaker segfault_runtime nlohmann_json::nlohmann_json)

set_target_properties(assetbaker PROPERTIES FOLDER tools\\assetbaker )
