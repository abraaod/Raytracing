cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

project(main)

add_executable(
    main
    ./src/main.cpp

    ./include/parser.hpp
    ./include/background.hpp
    ./include/film.hpp
    ./include/camera.hpp
    ./include/ray.hpp
    ./include/paramset.hpp
    ./include/api.hpp
    ./include/material.hpp 
    ./include/flatmaterial.hpp
    ./include/point2.hpp
    ./include/primitive.hpp
    ./include/scene.hpp
    ./include/sphere.hpp
    ./include/shape.hpp
    ./include/surfel.hpp
    ./include/geometricprimitive.hpp
    ./include/vec.hpp
    ./include/light/light.hpp
    ./include/light/ambient.hpp
    ./include/light/directional.hpp
    ./include/light/point.hpp
    ./include/light/spot.hpp
    ./include/bounds3.hpp
    ./include/triangle.hpp
    
    ./lib/pugixml.cpp
    ./lib/lodepng.cpp
    ./lib/tiny_obj_loader.cc
)