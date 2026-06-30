add_library(example_common STATIC
    examplebase.h
    examplebase.cpp
)

target_link_libraries(example_common PUBLIC segfault_runtime)

# Allow examples to include the base via "common/examplebase.h".
target_include_directories(example_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

set_target_properties(example_common PROPERTIES FOLDER examples)
