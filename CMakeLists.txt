cmake_minimum_required(VERSION 3.10)

project(frhdGen)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Source files
file(GLOB SOURCES
    src/*.cpp
)

add_executable(image2track ${SOURCES})

find_package(OpenCV 4 REQUIRED)

target_link_libraries(image2track ${OpenCV_LIBS})
