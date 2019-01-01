cmake_minimum_required(VERSION 3.9)
project(amethyst)

add_compile_options(-Wall -Wextra -pedantic-errors -O3 -DAMETHYST_DEBUG)
set(CMAKE_CXX_STANDARD 17)

add_library(amethyst SHARED
        src/socket.cpp
        src/socket.hpp
        src/socket.inl
        src/utility/platform.hpp
        src/amethyst.hpp
        src/amethyst.inl
        src/networking.hpp
        src/address.cpp
        src/address.hpp)
target_include_directories(amethyst PRIVATE ${PROJECT_SOURCE_DIR}/src)
if(WIN32)
    target_link_libraries(amethyst wsock32 ws2_32)
endif(WIN32)
# Unix requires no additional linking (Apparantly)

add_executable(amethyst_demo_http
        src/test/demo_http.cpp)
target_include_directories(amethyst_demo_http PUBLIC ${PROJECT_SOURCE_DIR}/src)
if(WIN32)
    target_link_libraries(amethyst_demo_http amethyst wsock32 ws2_32)
endif(WIN32)

add_executable(amethyst_demo_tcp
        src/test/demo_tcp.cpp)
target_include_directories(amethyst_demo_tcp PUBLIC ${PROJECT_SOURCE_DIR}/src)
if(WIN32)
    target_link_libraries(amethyst_demo_tcp amethyst wsock32 ws2_32)
endif(WIN32)

add_executable(amethyst_demo_udp
        src/test/demo_udp.cpp)
target_include_directories(amethyst_demo_udp PUBLIC ${PROJECT_SOURCE_DIR}/src)
if(WIN32)
    target_link_libraries(amethyst_demo_udp amethyst wsock32 ws2_32)
endif(WIN32)