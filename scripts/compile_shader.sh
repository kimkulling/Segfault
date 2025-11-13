#!/bin/bash
#

compile_shader() {
    cp ../assets/shaders/* .
    glslc default.vert -o vert.spv
    glslc default.frag -o frag.spv
    mkdir -p shaders
    mv *spv shaders
    rm default.vert
    rm default.frag
}

compile_shader

