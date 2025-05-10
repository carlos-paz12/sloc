cmake_minimum_required(VERSION 3.5)

project(SlotProject VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(APP_NAME "sloc")
add_executable(${APP_NAME} "src/app/main.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O2 -g")

target_include_directories(${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_include_directories(${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/common)
target_include_directories(${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/core/filter)
target_include_directories(${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/core/options)
target_include_directories(${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/core/sloc)
target_include_directories(${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/core/sort)

target_compile_features(${APP_NAME} PUBLIC cxx_std_17)
