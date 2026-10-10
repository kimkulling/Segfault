# Documentation configuration for Segfault
# This file sets up the Doxygen documentation target

# Find Doxygen
find_package(Doxygen QUIET)

if(DOXYGEN_FOUND)
    message(STATUS "Doxygen found: ${DOXYGEN_EXECUTABLE}")
    
    # Configure Doxygen
    set(DOXYGEN_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/../src/runtime 
                      ${CMAKE_CURRENT_SOURCE_DIR}/../src/contrib/cppcore/include
                      ${CMAKE_CURRENT_SOURCE_DIR}/Architecture.md)
    
    # Set output directory
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/html)
    
    # Create output directory before running Doxygen
    file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
    
    # Configure Doxyfile path with absolute paths
    set(DOXYGEN_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.configured)
    
    # Create a configured Doxyfile with absolute paths
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        ${DOXYGEN_CONFIG_FILE}
        @ONLY
    )
    
    # Add custom target for documentation
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
    
    # Add a clean target for documentation
    add_custom_target(doc-clean
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/html
        COMMENT "Cleaning Doxygen documentation"
        VERBATIM
    )
    
    # Make doc-clean depend on doc (optional)
    add_dependencies(doc-clean doc)
    
    message(STATUS "Documentation target 'doc' available - run 'cmake --build . --target doc' to generate")
else()
    message(WARNING "Doxygen not found. Documentation target 'doc' will not be available.")
    
    # Create a dummy target so the build doesn't fail
    add_custom_target(doc
        COMMAND ${CMAKE_COMMAND} -E echo "Doxygen not found. Please install Doxygen to generate documentation."
        COMMENT "Doxygen not available - skipping documentation generation"
        VERBATIM
    )
endif()
