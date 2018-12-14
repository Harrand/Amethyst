cmake_minimum_required(VERSION 3.9)
project(amethyst)

set(CMAKE_CXX_STANDARD 17)

add_library(amethyst SHARED
        src/socket.cpp
        src/socket.hpp
        src/socket.inl
        src/utility/platform.hpp
        src/amethyst.hpp
        src/amethyst.inl src/networking.hpp)
if(WIN32)
    target_link_libraries(amethyst wsock32 ws2_32)
endif(WIN32)