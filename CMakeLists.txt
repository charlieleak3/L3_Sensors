cmake_minimum_required(VERSION 3.14)
project(L3_Sensors VERSION 1.0.0 LANGUAGES C CXX)

# Set C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile_commands.json for editor IntelliSense (clangd / VS Code)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define public header directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Gather sensor driver source files
file(GLOB_RECURSE SENSOR_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Build the main sensor driver library
add_library(l3_sensors ${SENSOR_SOURCES})

# Target include directories for downstream linkage
target_include_directories(l3_sensors PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# ------------------------------------------------------------------------------
# EXAMPLES / DEMOS
# ------------------------------------------------------------------------------

# IR Breakbeam Demo
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/ir_breakbeam_demo/main.cpp")
    add_executable(ir_breakbeam_demo examples/ir_breakbeam_demo/main.cpp)
    target_link_libraries(ir_breakbeam_demo PRIVATE l3_sensors)
endif()

# Ultrasonic Demo
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/ultrasonic_demo/main.cpp")
    add_executable(ultrasonic_demo examples/ultrasonic_demo/main.cpp)
    target_link_libraries(ultrasonic_demo PRIVATE l3_sensors)
endif()

# ------------------------------------------------------------------------------
# UNIT TESTS (OPTIONAL)
# ------------------------------------------------------------------------------
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()