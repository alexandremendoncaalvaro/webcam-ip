cmake_minimum_required(VERSION 3.10)
project(webcam-ip)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Threads REQUIRED)

# Include Crow
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)

add_executable(webcam-ip webcam-ip.cpp)

target_link_libraries(webcam-ip 
    ${OpenCV_LIBS}
    Boost::program_options
    Threads::Threads
)

# Add any additional include directories if needed
target_include_directories(webcam-ip PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)