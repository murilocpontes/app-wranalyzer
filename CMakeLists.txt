cmake_minimum_required(VERSION 3.16.0)

# Project setup
project(WRAnalyzer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Qt configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
cmake_policy(SET CMP0100 NEW)  # Process .hh headers for Qt

# Dependency finding
find_package(Qt6 COMPONENTS Widgets Gui Core Network Concurrent REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(ZLIB REQUIRED)

# Path configuration
set(src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Protocol Buffer generation
if(EXISTS "${src_dir}/3rdparty/proto/protoc.sh")
    execute_process(COMMAND sh protoc.sh WORKING_DIRECTORY ${src_dir}/3rdparty/proto)
endif()
if(EXISTS "${src_dir}/proto/protoc.sh")
    execute_process(COMMAND sh protoc.sh WORKING_DIRECTORY ${src_dir}/proto)
endif()

# ------------------------------------------------------------
# Main Executable
# ------------------------------------------------------------
file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    ${src_dir}/*.c
    ${src_dir}/*.cpp
    ${src_dir}/*.cc
)

file(GLOB_RECURSE HEADERS
    CONFIGURE_DEPENDS
    ${src_dir}/*.h
    ${src_dir}/*.hpp
    ${src_dir}/*.hh
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${src_dir}
    ${src_dir}/3rdparty
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::Gui
    Qt6::Network
    Qt6::Core
    Qt6::Concurrent
    omniORB4
    omnithread
    protobuf
    GEARSystem
    Eigen3::Eigen
    ZLIB::ZLIB
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -isystem ${src_dir}/3rdparty
    -Wall
    -Wextra
)

# ------------------------------------------------------------
# Build Configuration
# ------------------------------------------------------------
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE QT_NO_DEBUG_OUTPUT QT_NO_INFO_OUTPUT QT_NO_WARNING_OUTPUT
    )
endif()
