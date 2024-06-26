cmake_minimum_required(VERSION 3.27)
project(3D_Graph_Modeling_basis)

set(CMAKE_CXX_STANDARD 17)

# Find the SFML package
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

# Add the executable
add_executable(3D_Graph_Modeling_basis main.cpp
        scale.h
        rotation.h
        draw.h
        function.h
        axes.h)

# Link SFML libraries
target_link_libraries(3D_Graph_Modeling_basis sfml-system sfml-window sfml-graphics sfml-network sfml-audio)