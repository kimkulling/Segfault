add_subdirectory(behaviortree_sample)
add_subdirectory(common)
add_subdirectory(hello_world)
add_subdirectory(scene_sample)
add_subdirectory(scene_sample_2d)

# Make all example executables depend on assets being copied
# This ensures that when examples are built, assets are available in the build directory
set(EXAMPLE_TARGETS hello_world behaviortree_sample scene_sample scene_sample_2d)
foreach(example_target ${EXAMPLE_TARGETS})
    if(TARGET ${example_target})
        add_dependencies(${example_target} copy_assets)
    endif()
endforeach()
