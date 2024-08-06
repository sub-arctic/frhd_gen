cmake_minimum_required(VERSION 3.10)

project(frhdGen)

find_package(OpenCV 4 REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(TrackCodeGenerator 
    main.cpp 
)

target_link_libraries(image2track ${OpenCV_LIBS})
